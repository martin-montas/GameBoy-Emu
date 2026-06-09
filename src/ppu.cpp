// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#include <stdio.h>

#include "ppu.hpp"
#include "mmu.hpp"

void Ppu::enter_mode_0() {
    if (_dot_clock == 456) {
        _dot_clock = 0;
        LY         = _mmu->read8(0xFF44);
        LY += 1;
        _mmu->write8(0xFF44, LY);
    }
}

void Ppu::enter_mode_3() {
    if (LCDC & FLAG_BG_ENABLE) {
        if (!(LCDC & FLAG_BG_MAP)) {
            for (int x = 0; x <= 159; x++) {
                uint8_t _scy         = _mmu->read8(0xFF42);
                uint8_t _scx         = _mmu->read8(0xFF43);
                uint8_t background_x = (x + _scx);
                uint8_t background_y = (LY + _scy);
                int     tile_x       = background_x / 8;
                int     tile_y       = background_y / 8;
                int     offset       = (tile_y * 32 + tile_x);

                //  gets tile number from tile map
                uint8_t  tile_number    = _mmu->read8(0x9800 + offset);
                uint16_t tile_data_addr = (0x8000 + tile_number * 16);

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
            // enter HBLANK_MODE here
        } else {
            for (int x = 0; x <= 159; x++) {
                uint8_t _scy         = _mmu->read8(0xFF42);
                uint8_t _scx         = _mmu->read8(0xFF43);
                uint8_t background_x = (x + _scx);
                uint8_t background_y = (LY + _scy);
                int     tile_x       = background_x / 8;
                int     tile_y       = background_y / 8;
                int     offset       = (tile_y * 32 + tile_x);

                //  gets tile number from tile map
                uint8_t  tile_number    = _mmu->read8(0x9C00 + offset);
                uint16_t tile_data_addr = (0x8000 + tile_number * 16);

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
    }
}

void Ppu::enter_mode_1() {
    // _dot_counter  resets
    if (_dot_clock >= 456) {
        _dot_clock = 0;
    }
}

void Ppu::mode_handler(int t_cycle) {
    LY = _mmu->read8(0xFF44);
    _dot_clock += t_cycle;

    uint8_t _lcd = _mmu->read8(0xFF40);
    if (_lcd & FLAG_LCD_ENABLE) {
        if (LY >= 144) {
            _mode = 1;
        } else {
            if (_dot_clock < 80) {
                _mode = 2;
            } else if (_dot_clock < 252) {
                _mode = 3;
            } else {
                _mode = 0;
            }
        }
    }
}

void Ppu::enter_mode_2() {
    //  Beginning of line -> Mode 2
    // oams its located at 0xFE00 - 0xFE9F
    return;
}

// void Ppu::enter_j() {
//     // foobar
//     return;
// }

void Ppu::dot_cycle(int t_cycle) {
    // handles dot_clock:
    if ((LCDC & FLAG_LCD_ENABLE) == 0) {
        return;
    }
    mode_handler(t_cycle);

    switch (_mode) {
    case 0: /* hblank */
        enter_mode_0();
        break;

    case 1: /* vblank */
        enter_mode_1();
        break;

    case 2: /* OAM Scan */
        enter_mode_2();
        break;

    case 3: /* Display */
        enter_mode_3();
        break;
    }
}
