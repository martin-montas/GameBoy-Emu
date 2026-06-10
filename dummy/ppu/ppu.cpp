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

    window = SDL_CreateWindow("SDL TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH * 2,
                              HEIGHT * 2, SDL_WINDOW_SHOWN);

    if (!window) {
        printf("Window Error: %s\n", SDL_GetError());
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    if (!renderer) {
        printf("Renderer Error: %s\n", SDL_GetError());
        exit(1);
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                WIDTH, HEIGHT);

    // clear framebuffer
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        buff[i] = 0xFF000000; // black
    }

    running = true;
}
void PPU::draw() {

    int x = 8;
    int y = 8;

    buff[y * WIDTH + x] = 0xFFFFFFFF;
}

void PPU::sdl_init() {
    return;
}

void PPU::step() {

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            running = false;
    }

    draw();

    SDL_UpdateTexture(texture, nullptr, buff, WIDTH * sizeof(uint32_t));

    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texture, nullptr, nullptr);

    SDL_RenderPresent(renderer);
}

int  cycles[] = {1, 3, 3, 4, 1, 1, 2, 2};
void PPU::run() {
    size_t indx = 0;
    while (running) {
        step();
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
