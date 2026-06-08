// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#include <cstdio>

#include "ppu.hpp"

PPU::PPU() {
    _cycles = 0;
    _mode   = MODE_OAM_SCAN;
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
    running = true;
}

void PPU::sdl_init() {
    return;
}

void PPU::draw() {
    // buff[y * WIDTH + x]
    buff[8 * WIDTH + 8] = 0xFFFFFFFF;
}

void PPU::step(int t_cycle) {
    SDL_Event event;
    _cycles += t_cycle;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            running = false;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_UpdateTexture(texture, nullptr, buff, WIDTH * sizeof(uint32_t));

    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
    SDL_Delay(32);
}

int  cycles[] = {1, 3, 3, 4, 1, 1, 2, 2};
void PPU::run() {
    size_t indx = 0;
    while (running) {
        step(cycles[indx]);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
