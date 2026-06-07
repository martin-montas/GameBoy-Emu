// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#ifndef SRC_PPU_HPP_
#define SRC_PPU_HPP_

#include <SDL2/SDL.h>
#include "mmu.hpp"
#include <cstdio>

enum PpuMode { MODE_HBLANK = 0, MODE_VBLANK = 1, MODE_OAM_SCAN = 2, MODE_DRAWING = 3 };

#define WHITE      #FFFFFF
#define light_gray #AAAAAA
#define dark_gray  #555555
#define black      #000000

struct Tile {
    uint8_t pixels[8][8];

}

class PPU {

    /*      Mode  Name What happens
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
     */

  private:
    Tile    frambuffer[160][144];
    MMU*    mmu;
    int     _cycle;
    PpuMode _mode;

  public:
    PPU(MMU mmu) : mmu(mmu), _cycle(0), _mode(MODE_OAM_SCAN);
    void draw();
    void oam_scan();
    void h_blank();
    void v_blank();
    void step(int t_cycle);
    void sdl_init();
};
#endif // SRC_PPU_HPP_
