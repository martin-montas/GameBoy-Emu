// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#include <stdio.h>

#include "ppu.hpp"
#include "mmu.hpp"

PPU::PPU(MMU* mmu) {
    mmu               = mmu;
    _mode             = MODE_OAM_SCAN;
    _scanline_counter = 0;
}

void PPU::sdl_init() {
    return;
}

void PPU::draw() {
    // buff[8 * WIDTH + 8] = 0xFFFFFFFF;
}
void PPU::mode_handler(int t_cycle) {
    _ly = mmu->read8(0xFF44);
    _dot_counter += t_cycle;

    // If LY >= 144
    //     Mode 1 (VBlank)
    // Else
    //     Use dot counter:
    //         Beginning of line -> Mode 2
    //         Middle of line    -> Mode 3
    //         End of line       -> Mode 0

    if (_ly >= 144)
        _mode = MODE_1;
    else {
        if (_dot_counter <= 79) {
            _mode = MODE_2;
        } else if (_dot_counter >= 80 && _dot_counter <= 251)
            _mode = MODE_3;
        else {
            _mode = MODE_0;
        }
    }
}

void PPU::_handler() {
    // oams its located at 0xFE00 - 0xFE9F
}

void PPU::step(int t_cycle) {
    // Mode 2
    //     Scan OAM for visible sprites
    //
    // Mode 3
    //     Read tile map
    //     Read tile number
    //     Check LCDC bit 4
    //     Calculate tile data address
    //     Fetch tile graphics
    //     Generate pixels
    //
    // Mode 0
    //     HBlank
    //
    // Mode 1
    //     VBlank
    //
    _ldc = mmu->read8(0xFF40);
    _ly  = mmu->read8(0xFF44);

    // handles _ly:
    mode_handler(t_cycle);

    switch (_mode) {
    case MODE_2: {
        oam_event_handler();
        break;
    }
    case MODE_3: {
        draw();
        break;
    }
    case MODE_1: {
        _dot_counter = 0;
        vblank_event_handler();
        break;
    }
    case MODE_0: {
        hblank_event_handler();
        break;
    }
    }
    // SDL_Event event;
    // while (SDL_PollEvent(&event)) {
    //     if (event.type == SDL_QUIT)
    //         ppu_running = false;

    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    // SDL_RenderClear(renderer);
    // SDL_UpdateTexture(texture, nullptr, buff, WIDTH * sizeof(uint32_t));

    // SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    // SDL_RenderPresent(renderer);
    // SDL_Delay(32);
}
