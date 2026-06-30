// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#include "ppu.hpp"

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
                // _interrupt->request_interrupt(INTERRUPT_VBLANK);
            } else if (LY == 154) {
                /* enters new frame */
                enter_mode_2();
            }
        }
    }
}

/*
 * @brief: Fetches pixels from tile data
 * and current tile map. and updates
 * the frame buffer array.
 *
 * @param[in]: address of selected tile map
 */
void Ppu::update_framebuff() {
    bool    window            = false;
    int     rendered_px       = 0;
    uint8_t _wy               = _mmu->read8(0xFF4A);
    uint8_t _wx               = _mmu->read8(0xFF4B);
    uint8_t LCDC              = _mmu->read8(0xFF40);
    bool    windown_v_trigger = LY >= _wy;
    bool    win_used          = false;

    // TODO try to werite the window:
    // please use WY register for it.

    for (int x = 0; x < 160; x++) {
        // if (windown_v_trigger && (x >= _wx - 7)) {
        // if (!(LCDC & FLAG_WIN_ENABLE)) {
        //     continue;
        // }
        // uint8_t px_x = x - (_wx - 7);
        // uint8_t px_y =  wl_counter;

        // /* used to get which tile the curent
        //  * pixel belongs to
        //  */
        // int tile_x = px_x / 8;
        // int tile_y = px_y / 8;

        // /* get tile map index with offset */
        // int     offset = tile_y * 32 + tile_x;
        // uint8_t tile_index;

        // /* fetches window tile  map index with offset */
        // tile_index = _mmu->read8(addr + offset);

        // uint16_t tile_data_addr;

        // if ((LCDC & FLAG_BG_AREA) == 0) {
        //     tile_data_addr = 0x9000 + (tile_index * 16);
        // } else {
        //     tile_index     = (int8_t)tile_index;
        //     tile_data_addr = 0x8000 + (tile_index * 16);
        // }

        // int     pixel_y = px_y % 8;
        // int     pixel_x = px_x % 8;
        // uint8_t byte0   = _mmu->read8(tile_data_addr + (pixel_x * 2));
        // uint8_t byte1   = _mmu->read8(tile_data_addr + (pixel_y * 2) + 1);
        // bool    msb     = ((byte0 >> (7 - px_y)) & 1);
        // bool    lsb     = ((byte1 >> (7 - px_x)) & 1);
        // uint8_t color   = (msb << 1) | lsb;

        // uint32_t color_val;
        // if (color == 0) {
        //     color_val = WHITE;
        // } else if (color == 1) {
        //     color_val = LIGHT_GRAY;
        // } else if (color == 2) {
        //     color_val = DARK_GRAY;
        // } else {
        //     color_val = BLACK;
        // }
        // frame_buff[LY * WIDTH + x] = color_val;
        // }
        // else {
        uint8_t _scy         = _mmu->read8(0xFF42);
        uint8_t _scx         = _mmu->read8(0xFF43);
        uint8_t background_x = (x + _scx);
        uint8_t background_y = (LY + _scy);
        int     tile_x       = background_x / 8;
        int     tile_y       = background_y / 8;

        /* fetches offset for the tile map */
        int      offset = (tile_y * 32 + tile_x);
        uint16_t addr;

        if (!(_LCDC & FLAG_BG_MAP)) {
            addr = 0x9800;
        } else {
            addr = 0x9C00;
        }
        /* gets tile number from tile map */
        uint8_t  tile_number = _mmu->read8(addr + offset);
        uint16_t tile_data_addr;

        /* gets tile addressing mode */
        if ((LCDC & FLAG_BG_AREA) == 0) {
            tile_data_addr = 0x9000 + (tile_number * 16);
        } else {
            tile_number    = (int8_t)tile_number;
            tile_data_addr = 0x8000 + (tile_number * 16);
        }

        int pixel_y = background_y % 8;
        int pixel_x = background_x % 8;

        uint8_t byte0 = _mmu->read8(tile_data_addr + (pixel_y * 2));
        uint8_t byte1 = _mmu->read8(tile_data_addr + (pixel_y * 2) + 1);

        bool msb = ((byte0 >> (7 - pixel_x)) & 1);
        bool lsb = ((byte1 >> (7 - pixel_x)) & 1);

        uint8_t  color = (msb << 1) | lsb;
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

void Ppu::enter_mode_3() {
    update_framebuff();
}

void Ppu::enter_mode_2() {
    /* Beginning of line -> Mode 2
     * oams its located at 0xFE00 - 0xFE9F
     */
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
    /* returns of _LCDC flag is set to 0 */
    _LCDC = _mmu->read8(LCDC_ADDR);
    if ((_LCDC & FLAG_LCD_ENABLE) == 0) {
        LY         = 0;
        wl_counter = 0;
        _mode      = 0;
        _dot_clock = 0;
        return;
    }
    _dot_clock += t_cycle;

    if ((_LCDC & FLAG_WIN_ENABLE) && (_LCDC & FLAG_WIN_ENABLE))
        _f_flag = win;

    else if (!(_LCDC & FLAG_WIN_ENABLE) && (_LCDC & FLAG_BG_ENABLE))
        _f_flag = bg;
    else
        _f_flag = obj;

    switch (_mode) {
    case 0: /* HBlank */
        if (_dot_clock >= 204) {
            _dot_clock -= 204;
            LY += 1;
            if (LY == 144) {
                wl_counter = 0;
                _mode      = 1;
                can_render = true;

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
            _mode     = 3;
            _win_line = 0;
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
