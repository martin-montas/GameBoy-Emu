// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#include <stdio.h>

#include "ppu.hpp"
#include "mmu.hpp"

PPU::PPU(MMU* mmu) {
    mmu               = mmu;
    _mode             = 2;
    _scanline_counter = 0;
}

void PPU::sdl_init() {
    return;
}

void PPU::pixel_renderer() {
    // Mode 3
    // Screen pixel x=0
    //     ↓
    // Find background coordinate (x + SCX, LY + SCY)
    //     ↓
    // Find which tile contains that coordinate
    //     ↓
    // Get that tile's pixel
    //     ↓
    // Draw it

    // Screen pixel x=1
    //     ↓
    // Do the same thing

    // ...
    // Screen pixel x=159

    uint8_t _lcd = mmu->read8(0xFF40);
    if (_lcd & FLAG_BG_ENABLE) {
        if (!(_lcd & FLAG_BG_MAP)) {
            for (int x = 0; x <= 159; x++) {
                uint8_t _scy         = mmu->read8(0xFF42);
                uint8_t _scx         = mmu->read8(0xFF43);
                uint8_t background_x = (x + _scx);
                uint8_t background_y = (_ly + _scy);
                int     tile_x       = background_x / 8;
                int     tile_y       = background_y / 8;
                int     offset       = (tile_y * 32 + tile_x);

                //  gets tile number from tile map
                uint8_t  tile_number    = mmu->read8(0x9800 + offset);
                uint16_t tile_data_addr = (0x8000 + tile_number * 16);

                int pixel_y = background_y % 8;
                int pixel_x = background_x % 8;

                uint8_t byte0 = mmu->read8(tile_data_addr + (pixel_y * 2));
                uint8_t byte1 = mmu->read8(tile_data_addr + (pixel_y * 2) + 1);

                bool msb = ((byte0 >> (7 - pixel_x)) & 1;
                bool lsb = ((byte1 >> (7 - pixel_x)) & 1;

                uint8_t color = (msb << 1) | lsb;
                uint32_t color_val;
                if (color == 0) {
                    color_val = WHITE;
                    }
                else if (color == 1) {
                    color_val = LIGHT_GRAY;
                    }
                else if (color == 2) {
                    color_val = DARK_GRAY;
                    }
                else {
                    color_val = BLACK;
                }

                frame_buff[_ly * WIDTH +x] = color_val;
            }
            // enter HBLANK_MODE here
            _ly += 1;
            mmu->write8(0xFF44, _ly);
        } else {
            for (int x = 0; x <= 159; x++) {
                uint8_t _scy         = mmu->read8(0xFF42);
                uint8_t _scx         = mmu->read8(0xFF43);
                uint8_t background_x = (x + _scx);
                uint8_t background_y = (_ly + _scy);
                int     tile_x       = background_x / 8;
                int     tile_y       = background_y / 8;
                int     offset       = (tile_y * 32 + tile_x);

                //  gets tile number from tile map
                uint8_t  tile_number    = mmu->read8(0x9C00 + offset);
                uint16_t tile_data_addr = (0x8000 + tile_number * 16);

                int pixel_y = background_y % 8;
                int pixel_x = background_x % 8;

                uint8_t byte0 = mmu->read8(tile_data_addr + (pixel_y * 2));
                uint8_t byte1 = mmu->read8(tile_data_addr + (pixel_y * 2) + 1);

                bool msb = ((byte0 >> (7 - pixel_x)) & 1;
                bool lsb = ((byte1 >> (7 - pixel_x)) & 1;

                uint8_t color = (msb << 1) | lsb;
                uint32_t color_val;

                if (color == 0) {
                    color_val = WHITE;
                    }
                else if (color == 1) {
                    color_val = LIGHT_GRAY;
                    }
                else if (color == 2) {
                    color_val = DARK_GRAY;
                    }
                else {
                    color_val = BLACK;
                }

                frame_buff[_ly * WIDTH +x] = color_val;
            }
            _ly += 1;
            mmu->write8(0xFF44, _ly);
        }
    }
}

void PPU::vblank_event_handler() {
    // _dot_counter  resets
    _dot_counter = 0;
}

// I should do something like this:
// void Ppu::read_reg(uint8_t& data, uint16_t addr) const {
//     switch (addr & 0xf) {
//     case 0x0:
//         data = LCDC;
//         break; /* ff40 */
//     case 0x1:
//         data = STAT | 0x80; /* ff41 */
//         if ((data & LCDC_ENABLE) != 0) {
//             data |= _mode;
//         }
//         break;
//     case 0x2:
//         data = SCY;
//         break; /* ff42 */
//     case 0x3:
//         data = SCX;
//         break; /* ff43 */
//     case 0x4:
//         data = LY;
//         break; /* ff44 */
//     case 0x5:
//         data = LYC;
//         break; /* ff45 */
//     case 0x6:
//         if (_mem) {
//             _mem->read_dma(data);
//         }
//         break; /* ff46 DMA */
//     case 0x7:
//         data = BGP;
//         break; /* ff47 */
//     case 0x8:
//         data = OBP0;
//         break; /* ff48 */
//     case 0x9:
//         data = OBP1;
//         break; /* ff49 */
//     case 0xa:
//         data = WY;
//         break; /* ff4a */
//     case 0xb:
//         data = WX;
//         break; /* ff4b */
//     default:
//         data = 0xff;
//         break;
//     }
// }

// this too:
// void Ppu::write_reg(uint8_t data, uint16_t addr) {
//      switch(addr & 0xf) {
//      case 0x0:                          /* ff40 */
//                /* Check if we are enabling or disabling controller */
//                if (((LCDC ^ data) & LCDC_ENABLE) != 0) {
//                    if ((data & LCDC_ENABLE) != 0) {
//                        LX = LY = 0;
//                        check_lyc();
//                        enter_mode0(true);
//                        _dot_clock = 0;
//                        _starting = 2;
//                        cycle_cnt = 0;
//                    } else {
//                        LX = LY = 0;
//                        enter_mode0(false);
//                        _dot_clock = 0;
//                        _starting = 0;
//                    }
//                }
//                LCDC = data;
//                break;
//      case 0x1: STAT = (data & 0x78) | (STAT & STAT_LYC_F);    /* ff41 */
//                post_irq(PPU_IRQ);
//                break;
//      case 0x2: SCY = data; break;       /* ff42 */
//      case 0x3: SCX = data; break;       /* ff43 */
//      case 0x4: break;                   /* ff44 LY */
//      case 0x5: LYC = data;              /* ff45 */
//                if ((data & LCDC_ENABLE) != 0) {
//                    check_lyc();
//                }
//                break;
//      case 0x6: if (_mem) {
//                    _mem->write_dma(data);
//                }
//                break;                   /* ff46 DMA */
//      case 0x7: BGP = data;              /* ff47 */
//                if ((_ppu_mode & 0xc) != 0) {
//                    set_palette_bw(0, data, _color);
//                }
//                break;
//      case 0x8: OBP0 = data;             /* ff48 */
//                if ((_ppu_mode & 0xc) != 0) {
//                    set_palette_bw(0x4, data, _color);
//                }
//                break;
//      case 0x9: OBP1 = data;             /* ff49 */
//                if ((_ppu_mode & 0xc) != 0) {
//                    set_palette_bw(0x8, data, _color);
//                }
//                break;
//      case 0xa: WY = data; break;       /* ff4a */
//      case 0xb: WX = data; break;       /* ff4b */
//      default:
//                break;
//      }
// }

void PPU::mode_handler(int t_cycle) {
    _ly = mmu->read8(0xFF44);
    _dot_counter += t_cycle;

    uint8_t _lcd = mmu->read8(0xFF40);
    if (_lcd & FLAG_LCD_ENABLE) {
        if (_ly >= 144) {
            _mode = 1;
        } else if (_dot_counter <= 79) {
            _mode = 2;
        } else if (_dot_counter >= 80 && _dot_counter <= 251)
            _mode = 3;
        else {
            _mode = 0;
        }
    }
}

void PPU::oam_event_handler() {
    //  Beginning of line -> Mode 2
    // oams its located at 0xFE00 - 0xFE9F
}

void PPU::step(int t_cycle) {
    // Mode 2
    //     Scan OAM for visible sprites
    //
    //
    // Mode 0
    //     HBlank
    //
    // Mode 1
    //     VBlank
    //

    // handles _ly:
    mode_handler(t_cycle);

    switch (_mode) {
    case 2: {
        oam_event_handler();
        break;
    }
    case 3: {
        pixel_renderer();
        break;
    }
    case 1: {
        vblank_event_handler();
        break;
    }
    case 0: {
        hblank_event_handler();
        break;
    }
    }
}
