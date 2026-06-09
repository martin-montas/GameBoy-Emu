// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#ifndef SRC_PPU_HPP_
#define SRC_PPU_HPP_

/*
 *                           1. Tile Data
 *
 *     Tile data lives in memory starting from $8000 to $97FF.
 *     each of them is its 16 bytes. Each char/pixel of the tile
 *     its 8x8 pixels and each row its 2 bytes per row.
 *     Each color its represented by 2 bits. pixel 0 and others
 *     pixel colors are represented by the bit 0 of 1 byte and
 *     bit 0 of second byte in the current row.
 *
 *     examaple:
 *
 *      [ MSB ]    [ LSB ]
 *      -------------------
 *      00110010 | 11011010 -> row
 *      ...
 *      ...
 *
 *     given first row: bit 0 of first byte its 0 and bit 0 of second byte its 1.
 *     therefore the value [10] equals light gray. remember that the bytes are
 *     swapped since the most significat bits is the first byte of each row.
 *
 *                            2. Tile Maps
 *
 *   A tile map are 32x32 of values ranging from 0 to 255. each of these
 *   values correspond to the indices of the of the titles to be displayed.
 *
 *                      3. Background Rendering
 *
 *   There are 2 tile maps and the ppu needs to know which of them should be used.
 *   The value to do this its done through bit 3 of LCDC register. When this bit
 *   is 0 you use the tile map that starts at 0x9800 else if 1 use you the one
 *   that starts at 0x9C00.
 *
 *   To render the tiles on the background, you will to find the row and the col
 *   correspoding to the ly register found at 0xFF44. lets say that for example
 *   ly equals 25 then to find the row that that should be drawn, you will use this
 *   formula: tile_row = 25 / 8 = 3. Now, lets say you want to find the col, lets say
 *   col 37, you will use the same tile_col: 37 / 8 = 4, so in the row 3, col 4 of the
 *   given tile map on vram. since tile maps has 32 entries this formula holds true.
 *   tile_map_index = tile_row * 32 + tile_col.
 */

#include <stdio.h>
#include "mmu.hpp"

enum PpuMode { MODE_0 = 0, MODE_1 = 1, MODE_2 = 2, MODE_3 = 3 };

enum LCDFlag {
    FLAG_ENABLE     = (1 << 7),
    FLAG_WIN_AREA   = (1 << 6),
    FLAG_WIN_ENBLE  = (1 << 5),
    FLAG_BG_AREA    = (1 << 4),
    flag_bg_MAP     = (1 << 3),
    FLAG_OBJ_SIZE   = (1 << 2),
    FLAG_OBJ_ENABLE = (1 << 1),
    FLAG_BG_ENABLE  = 1
};

typedef struct {
    SDL_Rect pixel[8 * 8];
} Tile;

class PPU {

  private:
    uint32_t buff[WIDTH * HEIGHT];
    MMU*     mmu;
    size_t   _dot_counter;
    PpuMode  _mode;
    uint8_t  _ldc;
    uint8_t  _ly;
    uint8_t  _scanline_counter;
    /*
     * @brief: this happens on mode 3 of the ppu.
     * where the value at 0xFF40 is read and
     *
     */
    void draw();
    void mode_handler(int t_cycle);

    void oam_event_handler();
    void hblank_event_handler();
    void vblank_event_handler();

    ~PPU();

  public:
    PPU(MMU* mmu);
    void step(int t_cycle);
    void sdl_init();
};
#endif // SRC_PPU_HPP_
