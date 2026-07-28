// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#include "ppu.hpp"
#include <cstdio>
#include <algorithm>
#include <execution>

/*
 * @brief Handles hblank-related things like
 * moving the another scanline and
 * entering vblank.
 */
void Ppu::hblank_handler() {
    /* checks if scan line has ended */
    if (_f_flag == bg) {
        if (_dot_clock == 456) {
            _dot_clock = 0;
            LY += 1;
            /* enters vblank */
            if (LY == 144) {
                _interrupt->request_interrupt(INTERRUPT_VBLANK);
            } else if (LY == 154) {
                /* enters new frame */
                enter_mode_2();
            }
        }
    }
}

void Ppu::enter_mode_2() {
    scan_oam();
}

void Ppu::scan_oam() {}

void Ppu::update_obj_framebuff() {
    uint8_t LCDC = _bus->read8(0xFF40);
    if (!(LCDC & FLAG_OBJ_ENABLE)) {
        return;
    }
    int     offset;
    uint8_t LY   = _bus->read8(0xFF44);
    int     high = (LCDC & FLAG_OBJ_SIZE) ? 16 : 8;

    for (int obj = 0; obj < 10; obj++) {
        objs[obj].X = 0xff;
    }

    if ((LCDC & FLAG_OBJ_ENABLE) == 0) {
        return;
    }
    size_t obj_size     = 0;
    int    sprite_index = 0;
    for (sprite_index = 0; sprite_index < 40; sprite_index++) {
        uint16_t oam_addr   = 0xFE00 + (sprite_index * 4);
        uint8_t  sprite_y   = _bus->read8(oam_addr);
        uint8_t  sprite_x   = _bus->read8(oam_addr + 1);
        uint8_t  tile_index = _bus->read8(oam_addr + 2);
        uint8_t  attr       = _bus->read8(oam_addr + 3);
        uint8_t  actual_Y   = sprite_y - 16;
        uint8_t  actual_X   = sprite_x - 8;

        if ((LY >= actual_Y) && (LY < (actual_Y + high))) {
            objs[obj_size] = OBJ{actual_X, actual_Y, tile_index, attr, sprite_index};
            obj_size += 1;
        }
        if (obj_size >= 10) {
            obj_size = 0;
            break;
        }
    }

    LCDC         = _bus->read8(0xFF40);
    int obj_mode = (LCDC & FLAG_OBJ_SIZE);
    // std::stable_sort(objs.begin(), objs.begin() + sprite_index,
    //                  [](const OBJ& a, const OBJ& b) { return a.X < b.X; });

    std::sort(objs.begin(), objs.end(), [](const OBJ& a, const OBJ& b) {
        if (a.X == b.X)
            return a.oam_index < b.oam_index;
        return a.X < b.X;
    });
    for (int i = objs.size(); i >= 0; i--) {
        uint8_t sprite_row = LY - objs[i].Y;
        bool    flip_x     = (objs[i].attr & 0x20) != 0;
        bool    flip_y     = (objs[i].attr & 0x40) != 0;
        uint8_t tile_index = objs[i].tile_index;
        bool    belowbg    = (objs[i].attr & 0x80) != 0;
        int     tile_row   = flip_y ? (obj_mode - 1 - sprite_row) : sprite_row;

        if (obj_mode == 16) {
            tile_index &= 0xFE;
            if (tile_row >= 8) {
                tile_index += 1;
                tile_row -= 8;
            }
        }

        uint16_t tile_addr = 0x8000 + (tile_index * 16);

        uint8_t byte1 = _bus->read8(tile_addr + (tile_row * 2));
        uint8_t byte2 = _bus->read8(tile_addr + (tile_row * 2) + 1);

        for (int pixel_x = 0; pixel_x < 8; pixel_x++) {
            int screen_x = objs[i].X + pixel_x;

            if (screen_x < 0 || screen_x >= WIDTH) {
                continue;
            }
            int bit_index = flip_x ? pixel_x : (7 - pixel_x);

            bool bit1 = (byte1 >> bit_index) & 1;
            bool bit2 = (byte2 >> bit_index) & 1;

            uint8_t pixel_val = (bit2 << 1) | bit1;

            if (pixel_val == 0) {
                continue;
            }

            bool    use_obp1 = (objs[i].attr & 0x10) != 0;
            uint8_t palette  = _bus->read8(use_obp1 ? 0xFF49 : 0xFF48);
            uint8_t shade    = (palette >> (pixel_val * 2)) & 0x03;

            uint32_t color_val;
            switch (shade) {
            case 1:
                color_val = LIGHT_GRAY;
                break;
            case 2:
                color_val = DARK_GRAY;
                break;
            case 3:
                color_val = BLACK;
                break;
            }
            uint8_t bg_pixel = bg_tmp_buff[LY * WIDTH + screen_x];
            if (belowbg == 0) {
                frame_buff[LY * WIDTH + screen_x] = color_val;
            } else {
                // priority_bit == 1 ("behind BG")
                if (bg_pixel == 0) {
                    frame_buff[LY * WIDTH + screen_x] = color_val;
                }
            }
        }
    }
}

void Ppu::update_framebuff_blank() {
    for (int x = 0; x < 160; x++) {
        frame_buff[LY * WIDTH + x] = WHITE;
    }
}

/*
 * @brief: update frames for the bg layer
 *
 */
void Ppu::update_bg_framebuff() {
    for (int x = 0; x < 160; x++) {
        uint8_t _scy         = _bus->read8(0xFF42);
        uint8_t LCDC         = _bus->read8(0xFF40);
        uint8_t _scx         = _bus->read8(0xFF43);
        uint8_t background_x = (x + _scx);
        uint8_t background_y = (LY + _scy);
        int     tile_x       = background_x / 8;
        int     tile_y       = background_y / 8;

        int      offset = (tile_y * 32 + tile_x);
        uint16_t addr;

        if (!(_LCDC & FLAG_BG_MAP)) {
            addr = 0x9800;
        } else {
            addr = 0x9C00;
        }
        uint8_t  tile_number = _bus->read8(addr + offset);
        uint16_t tile_data_addr;

        if (LCDC & FLAG_BGWIN_AREA) {
            tile_data_addr = 0x8000 + (tile_number * 16);
        } else {
            int8_t signed_index = (int8_t)tile_number;
            tile_data_addr      = 0x9000 + (signed_index * 16);
        }
        int pixel_y = background_y % 8;
        int pixel_x = background_x % 8;

        uint8_t byte0 = _bus->read8(tile_data_addr + (pixel_y * 2));     // LSB
        uint8_t byte1 = _bus->read8(tile_data_addr + (pixel_y * 2) + 1); // MSB

        uint8_t bit_pos = 7 - pixel_x;
        bool    lsb     = (byte0 >> bit_pos) & 1;
        bool    msb     = (byte1 >> bit_pos) & 1;

        uint8_t color_idx = (msb << 1) | lsb;

        uint8_t bgp   = _bus->read8(0xFF47);
        uint8_t shade = (bgp >> (color_idx * 2)) & 0x03;

        uint32_t color_val;
        switch (shade) {
        case 0:
            color_val = WHITE;
            break;
        case 1:
            color_val = LIGHT_GRAY;
            break;
        case 2:
            color_val = DARK_GRAY;
            break;
        case 3:
            color_val = BLACK;
            break;
        }

        frame_buff[LY * WIDTH + x]  = color_val;
        bg_tmp_buff[LY * WIDTH + x] = color_idx;
    }
}

/*
 * @brief: update frames for the win layer
 *
 */
void Ppu::update_win_framebuff() {
    bool    window         = false;
    int     rendered_px    = 0;
    uint8_t _wy            = _bus->read8(0xFF4A);
    uint8_t _wx            = _bus->read8(0xFF4B);
    uint8_t LCDC           = _bus->read8(0xFF40);
    bool    window_enabled = LCDC & FLAG_WIN_ENABLE;
    bool    window_active  = window_enabled && (LY >= _wy);
    win_used               = false;
    for (int x = 0; x < 160; x++) {
        if (!(LCDC & FLAG_BGWIN_PRIORITY)) {
            update_framebuff_blank();
            return;
        }
        bool window_fires = window_active && (x >= _wx - 7);

        if (window_fires) {
            win_used     = true;
            uint8_t px_x = x - (_wx - 7);
            uint8_t px_y = wl_counter;

            int tile_x = px_x / 8;
            int tile_y = px_y / 8;

            int      offset = tile_y * 32 + tile_x;
            uint8_t  tile_index;
            uint16_t addr;

            if ((LCDC & FLAG_WIN_MAP) == 0) {
                addr = 0x9800;
            } else {
                addr = 0x9C00;
            }
            uint8_t  tile_number = _bus->read8(addr + offset);
            uint16_t tile_data_addr;

            if (_LCDC & FLAG_BGWIN_AREA) {
                tile_data_addr = 0x8000 + (tile_number * 16);
            } else {
                int8_t signed_index = (int8_t)tile_number;
                tile_data_addr      = 0x9000 + (signed_index * 16);
            }

            int pixel_y = px_y % 8; // Row within the 8x8 tile
            int pixel_x = px_x % 8; // Column within the 8x8 tile

            // 1. Fetch both bytes for row 'pixel_y'
            uint8_t byte0 = _bus->read8(tile_data_addr + (pixel_y * 2));     // LSB
            uint8_t byte1 = _bus->read8(tile_data_addr + (pixel_y * 2) + 1); // MSB

            // 2. Extract bits for column 'pixel_x' (Bit 7 is leftmost)
            uint8_t bit_pos = 7 - pixel_x;
            bool    lsb     = (byte0 >> bit_pos) & 1;
            bool    msb     = (byte1 >> bit_pos) & 1;

            uint8_t color_idx = (msb << 1) | lsb;

            // 3. Map through BGP register (0xFF47)
            uint8_t bgp   = _bus->read8(0xFF47);
            uint8_t shade = (bgp >> (color_idx * 2)) & 0x03;

            uint32_t color_val;
            switch (shade) {
            case 0:
                color_val = WHITE;
                break;
            case 1:
                color_val = LIGHT_GRAY;
                break;
            case 2:
                color_val = DARK_GRAY;
                break;
            case 3:
                color_val = BLACK;
                break;
            }

            frame_buff[LY * WIDTH + x]  = color_val;
            bg_tmp_buff[LY * WIDTH + x] = color_idx;
        }
    }
    if (win_used) {
        wl_counter += 1;
    }
}
/*
 * @brief: Fetches pixels from tile data
 * and current tile map. and updates
 * the frame buffer array.
 *
 */
void Ppu::render_scanline() {
    uint8_t LCDC = _bus->read8(0xFF40);
    uint8_t _wy  = _bus->read8(0xFF4A);
    uint8_t _wx  = _bus->read8(0xFF4B);

    if ((LCDC & FLAG_BGWIN_PRIORITY) != 0) {
        update_bg_framebuff();
    }
    if (((LCDC & FLAG_WIN_ENABLE) != 0) && LY >= _wy && _wx <= 166) {
        update_win_framebuff();
    }
    if ((LCDC & FLAG_OBJ_ENABLE) != 0) {
        update_obj_framebuff();
    }
}

void Ppu::fetch_sprites() {}

void Ppu::enter_mode_3() {
    render_scanline();
}

void Ppu::switch_mode(int mode) {
    if (mode == 0)
        hblank_handler();
    else if (mode == 2)
        enter_mode_2();
    else if (mode == 3)
        enter_mode_3();
}

bool Ppu::frame_ready() const {
    return can_render;
}

void Ppu::dot_cycle(int t_cycle) {
    _LCDC = _bus->read8(LCDC_ADDR);
    if ((_LCDC & FLAG_LCD_ENABLE) == 0) {
        LY         = 0;
        wl_counter = 0;
        _mode      = 0;
        _dot_clock = 0;
        win_used   = false;
        return;
    }
    _dot_clock += t_cycle;

    if ((_LCDC & FLAG_WIN_ENABLE) && (_LCDC & FLAG_WIN_ENABLE))
        _f_flag = win;

    else if (!(_LCDC & FLAG_WIN_ENABLE) && (_LCDC & FLAG_BGWIN_PRIORITY))
        _f_flag = bg;
    else
        _f_flag = obj;

    switch (_mode) {
    case 0: /* HBlank */
        if (_dot_clock >= 204) {
            _dot_clock -= 204;
            LY += 1;
            uint8_t LYC = _bus->read8(0xFF45);
            if (LY == LYC) {
                _interrupt->request_interrupt(INTERRUPT_LCD);
            }
            if (LY == 144) {
                wl_counter = 0;
                _mode      = 1;
                can_render = true;
                LY         = 0;
            } else {
                _mode = 2;
                switch_mode(2);
            }
        }
        break;
    case 1: /* vblank */
        if (_dot_clock >= 456) {
            _dot_clock -= 456;
            LY += 1;
            if (LY == 154) {
                _mode = 2;
                LY    = 0;
                switch_mode(2);
            }
        }
        break;
    case 2: /* oam scan */
        if (_dot_clock >= 80) {
            _dot_clock -= 80;
            _mode = 3;
            switch_mode(3);
        }
        break;
    case 3: /* render */
        if (_dot_clock >= 172) {
            _dot_clock -= 172;
            _mode = 0;
            switch_mode(0);
        }
        break;
    }
}
