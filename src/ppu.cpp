// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#include "ppu.hpp"
#include <cstdio>

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

void Ppu::scan_oam() {

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
    size_t obj_size = 0;
    for (int sprite_index = 0; sprite_index < 40; sprite_index++) {
        uint16_t oam_addr = 0xFE00 + (sprite_index * 4);

        uint8_t sprite_y   = _bus->read8(oam_addr);
        uint8_t sprite_x   = _bus->read8(oam_addr + 1);
        uint8_t tile_index = _bus->read8(oam_addr + 2);
        uint8_t attr       = _bus->read8(oam_addr + 3);

        uint8_t actual_Y = sprite_y - 16;
        uint8_t actual_X = sprite_x - 8;
        if (LY >= actual_Y && LY < actual_Y + high) {
            objs[obj_size] = OBJ{actual_X, actual_Y, tile_index, attr};
            obj_size += 1;
            if (obj_size >= 10) {
                obj_size = 0;
                break;
            }
        }
    }
}

void Ppu::update_obj_framebuff() {
    uint8_t LCDC     = _bus->read8(0xFF40);
    int     obj_mode = (LCDC & FLAG_OBJ_SIZE);

    std::sort(objs.begin(), objs.end(), [](const OBJ& a, const OBJ& b) {
        if (a.X == b.X)
            return a.tile_index < b.tile_index;
        return a.X < b.X;
    });
    for (int i = objs.size() - 1; i >= 0; i--) {
        uint8_t  sprite_row     = LY - objs[i].Y;
        bool     flip_x         = (objs[i].attr & 0x20) != 0;
        bool     flip_y         = (objs[i].attr & 0x40) != 0;
        uint8_t  tile_index     = objs[i].tile_index;
        bool     belowbg        = (objs[i].attr & 0x80) != 0;
        int      tile_row       = flip_y ? (obj_mode - 1 - sprite_row) : sprite_row;
        uint16_t actual_address = tile_index;
        if (obj_mode == 16) {
            if (tile_row >= 8) {
                actual_address = tile_index & 0xFE;
            } else {
                actual_address = tile_index | 0x01;
                tile_row -= 8;
            }
        }
        uint16_t tile_addr = 0x8000 + (actual_address * 16);

        uint8_t byte1 = _bus->read8(tile_addr + (tile_row * 2));
        uint8_t byte2 = _bus->read8(tile_addr + (tile_row * 2) + 1);

        for (int pixel_x = 0; pixel_x < 8; pixel_x++) {
            int screen_x = objs[i].X + pixel_x;

            if (screen_x < 0 || screen_x >= WIDTH) {
                continue;
            }
            int  bit_index = flip_x ? pixel_x : (7 - pixel_x);
            bool bit1      = (byte1 >> bit_index) & 1;
            bool bit2      = (byte2 >> bit_index) & 1;

            uint8_t  pixel_val = (bit2 << 1) | bit1;
            uint32_t color_val;

            if (pixel_val == 0) {
                color_val = WHITE;
            } else if (pixel_val == 1) {
                color_val = LIGHT_GRAY;
            } else if (pixel_val == 2) {
                color_val = DARK_GRAY;
            } else {
                color_val = BLACK;
            }
            if (belowbg) {
                continue;
                uint32_t bg_pix = frame_buff[LY * WIDTH + screen_x];
                if (bg_pix != BLACK) {
                    continue;
                }
            }
            frame_buff[LY * WIDTH + screen_x] = color_val;
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

        if (_LCDC & FLAG_BGWIN_AREA) {
            tile_data_addr = 0x8000 + (tile_number * 16);
        } else {
            int8_t signed_index = (int8_t)tile_number;
            tile_data_addr      = 0x9000 + (signed_index * 16);
        }
        int pixel_y = background_y % 8;
        int pixel_x = background_x % 8;

        uint8_t byte0 = _bus->read8(tile_data_addr + (pixel_y * 2));
        uint8_t byte1 = _bus->read8(tile_data_addr + (pixel_y * 2) + 1);

        bool msb = ((byte0 >> (7 - pixel_x)) & 1);
        bool lsb = ((byte1 >> (7 - pixel_x)) & 1);

        uint8_t  color = (lsb << 1) | msb;
        uint32_t color_val;
        if (color == 0) {
            color_val = WHITE;
        } else if (color == 1) {
            color_val = LIGHT_GRAY;
        } else if (color == 2) {
            color_val = DARK_GRAY;
        } else {
            color_val = BLACK;
        }
        frame_buff[LY * WIDTH + x] = color_val;
    }
}

/*
 * @brief: update frames for the win layer
 *
 */

//     fn render_window_scanline(&mut self) {
//         let line = self.ly as usize;
//
//         let tilemap_base: u16 = if self.lcdc & LCDC_WIN_TILEMAP != 0 {
//             0x9C00
//         } else {
//             0x9800
//         };
//
//         let unsigned_addressing = self.lcdc & LCDC_BG_WIN_TILEDATA != 0;
//
//         // The window X on screen starts at (WX - 7). Values 0-6 are
//         // effectively negative and clip the left side of the window.
//         let wx_screen = self.wx as i16 - 7;
//
//         let pixel_y = self.window_line % 8;
//         let tile_row = (self.window_line as u16 / 8) & 31;
//
//         let mut any_drawn = false;
//
//         for px in 0..SCREEN_WIDTH {
//             let screen_x = px as i16;
//             if screen_x < wx_screen {
//                 continue;
//             }
//
//             any_drawn = true;
//             let win_x = (screen_x - wx_screen) as u8;
//             let tile_col = (win_x as u16 / 8) & 31;
//             let pixel_x = win_x % 8;
//
//             let map_addr = tilemap_base + tile_row * 32 + tile_col;
//             let tile_index = self.vram_read_internal(map_addr);
//
//             let tile_data_addr = tile_data_address(tile_index, unsigned_addressing, pixel_y);
//
//             let lo = self.vram_read_internal(tile_data_addr);
//             let hi = self.vram_read_internal(tile_data_addr + 1);
//
//             let colour_id = pixel_colour_id(lo, hi, pixel_x);
//             self.bg_color_ids[px] = colour_id;
//             self.frame_buf[line][px] = apply_palette(self.bgp, colour_id);
//         }
//
//         // The window line counter only increments on scanlines where the
//         // window was actually rendered.
//         if any_drawn {
//             self.window_line += 1;
//             self.window_triggered = true;
//         }
//     }

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

            if ((_LCDC & FLAG_WIN_MAP)) {
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

            int     pixel_y = px_y % 8;
            int     pixel_x = px_x % 8;
            uint8_t byte0   = _bus->read8(tile_data_addr + (pixel_x * 2));
            uint8_t byte1   = _bus->read8(tile_data_addr + (pixel_y * 2) + 1);
            bool    msb     = ((byte0 >> (7 - px_y)) & 1);
            bool    lsb     = ((byte1 >> (7 - px_x)) & 1);
            uint8_t color   = (msb << 1) | lsb;

            uint32_t color_val;
            if (color == 0) {
                color_val = WHITE;
            } else if (color == 1) {
                color_val = LIGHT_GRAY;
            } else if (color == 2) {
                color_val = DARK_GRAY;
            } else {
                color_val = BLACK;
            }
            frame_buff[LY * WIDTH + x] = color_val;
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
