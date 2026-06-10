// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#ifndef SRC_PPU_HPP_
#define SRC_PPU_HPP_

/*  Mode  Name What happens
 *
 *     Mode 2: OAM, Scan Search OAM for up to 10 sprites that will appear on the current
 *     line. Mode 3: Drawing, Fetch tile/sprite data and generate pixels. Mode 0: HBlank,
 *     Finished drawing the line, waiting for next line Mode 1: VBlank, Finished all visible
 *     lines, screen can be updated
 *
 *  During Mode 3 the PPU:
 *
 *     Reads the scroll registers (SCX, SCY).
 *     Determines which background/window tiles are visible.
 *     Fetches tile numbers from the tile map.
 *     Fetches tile graphics from tile data in VRAM.
 *     Pushes pixel data into a pixel FIFO.
 *     Checks sprites that overlap the current X position.
 *     Fetches sprite tile data when needed.
 *     Mixes sprite pixels with background pixels according to priority rules.
 *     Outputs one pixel at a time to the LCD.
 *
 *
 *  1. Tile Data
 *
 *     Tile data lives in memory starting from $8000 to $97FF.
 *     each of them is its 16 bytes. Each char/pixel of the tile
 *     its 8x8 pixels and each row its 2 bytes per row.
 *
 *     Each color its represented by 2 bits. pixel 0 and others
 *     pixel colors are represented by the bit 0 of 1 byte and
 *     bit 0 of second byte in the current row.
 *
 *     examaple:
 *
 *      [ MSB ]    [ LSB ]
 *      -------------------
 *      00110010 | 11011010 -> row
 *      00110010 | 01011010
 *      00110010 | 01011010
 *      00110010 | 01011010
 *      00110010 | 01011010
 *      00110010 | 01011010
 *      00110010 | 01011010
 *      00110010 | 01011010
 *
 *     given first row:
 *
 *      bit 0 of first byte its 0 and bit 0 of second byte its 1. therefore the value
 *      [10] equals light gray. remember that the bytes are swapped since the most
 *      significat bits is the first byte of each row.
 *
 *  2. Tile Maps
 *
 *   A tile map are 32x32 of values ranging from 0 to 255. each of these
 *   values correspond to the indices of the of the titles to be displayed.
 *
 *
 */

#include <SDL2/SDL.h>
#include <cstdio>

#define WIDTH  160
#define HEIGHT 144

enum PpuMode { MODE_HBLANK = 0, MODE_VBLANK = 1, MODE_OAM_SCAN = 2, MODE_DRAWING = 3 };

#define WHITE      0xFFFFFFFF
#define LIGHT_GRAY 0xFFAAAAAA
#define DARK_GRAY  0xFF555555
#define BLACK      0xFF000000
#define SCALE      4

typedef struct {
    SDL_Rect pixel[8 * 8];
} Tile;

class PPU {

  private:
    uint32_t      buff[WIDTH * HEIGHT];
    int           _cycles;
    PpuMode       _mode;
    uint16_t      _ldc;
    SDL_Renderer* renderer;
    SDL_Surface*  screen;
    SDL_Texture*  texture;
    SDL_Window*   window;
    bool          ppu_running;
    void          draw();
    void          oam_scan();
    void          h_blank();
    void          v_blank();
    bool          running;
    void          step();
    void          sdl_init();

  public:
    PPU();
    void run();
};
#endif // SRC_PPU_HPP_
