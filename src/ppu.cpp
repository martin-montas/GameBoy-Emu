// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#include <cstdio>

#include "ppu.hpp"
#include "mmu.hpp"

PPU::PPU(MMU* mmu) {
    mmu     = mmu;
    _cycles = 0;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL Error: %s\n", SDL_GetError());
        exit(1);
    }
    printf("Video driver: %s\n", SDL_GetCurrentVideoDriver());
    window = SDL_CreateWindow("SDL TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH * 2,
                              HEIGHT * 2, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window Error: %s\n", SDL_GetError());
        exit(1);
    }
    renderer             = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
    if (!renderer) {
        printf("Renderer Error: %s\n", SDL_GetError());
        exit(1);
    }
    bool running = true;
}

void PPU::sdl_init() {
    return;
}

PPU::~PPU() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void PPU::draw() {
    // buff[y * WIDTH + x]
    buff[8 * WIDTH + 8] = 0xFFFFFFFF;
}

void PPU::scanline_timing_handler() {
    if (_mode == MODE_VBLANK) {
        _ly = mmu->read8(0XFF44);
        _ly += 1;
        mmu->write8(0xFF44, _ly);
    }
}

void PPU::step(int t_cycle) {
    _ldc = mmu->read8(0XFF40);

    _cycles += t_cycle;

    if (_cycles == 455) {
        _ly = mmu->read8(0XFF44);
        _ly += 1;
        mmu->write8(0xFF44, _ly);
        _cycles = 0;
        // _mode   = mode
    }

    // scanline_timing_handler();

    // if (_cycles < 79) {
    //     // mode 2: oam scan
    //     _mode = MODE_OAM_SCAN

    // } else if (_cycles >= 80 && _cycles <= 251) {
    //     if (_cycles >= 144 && _cycles <= 153) {
    //         // switch mode 1:  vblank
    //         _mode = MODE_VBLANK

    //     } else {
    //         // switch mode 3: drawing
    //         _mode = MODE_DRAWING
    //     }
    // } else if (_cycles >= 252 && _cycles <= 455) {
    //     // switch mode 0: hblank
    //     _ly = mmu->read8(0XFF44);
    //     _ly += 1;
    //     mmu->write8(0xFF44, _ly);
    //     _mode = MODE_DRAWING;
    // } else {
    //     _cycles = 0;
    // }

    // SDL_Event event;
    // while (SDL_PollEvent(&event)) {
    //     if (event.type == SDL_QUIT)
    //         ppu_running = false;
    // }

    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    // SDL_RenderClear(renderer);
    // SDL_UpdateTexture(texture, nullptr, buff, WIDTH * sizeof(uint32_t));

    // SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    // SDL_RenderPresent(renderer);
    // SDL_Delay(32);
}
