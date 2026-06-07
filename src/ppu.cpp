// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
#include <SDL2/SDL.h>
#include <cstdio>

#include "ppu.hpp"

// ====================================================================
//                          -- for referece --
//
//
// ====================================================================
// #include <iostream>
// #include <cstdint>
// #include <SDL2/SDL.h>
//
// const int SCALE = 4;
//
// uint32_t framebuffer[160 * 144];
// int      main() {
//     if (SDL_Init(SDL_INIT_VIDEO) != 0) {
//         printf("SDL Error: %s\n", SDL_GetError());
//         return 1;
//     }
//
//     SDL_Window* window =
//         SDL_CreateWindow("SDL TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 160 * 2,
//                          144 * 2, SDL_WINDOW_SHOWN);
//
//     if (!window) {
//         printf("Window Error: %s\n", SDL_GetError());
//         return 1;
//     }
//
//     SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
//     SDL_Surface*  screen;
//     screen = SDL_GetWindowSurface(window);
//
//     SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
//                                              SDL_TEXTUREACCESS_STREAMING, 160, 144);
//
//     if (!renderer) {
//         printf("Renderer Error: %s\n", SDL_GetError());
//         return 1;
//     }
//
//     bool running = true;
//
//     SDL_Rect pixel           = {8 * SCALE, 8 * SCALE, SCALE, SCALE};
//     framebuffer[8 * 160 + 8] = 0xFFFFFFFF;
//
//     while (running) {
//         SDL_Event event;
//
//         while (SDL_PollEvent(&event)) {
//             if (event.type == SDL_QUIT)
//                 running = false;
//         }
//
//         SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
//         SDL_RenderClear(renderer);
//         SDL_UpdateTexture(texture, nullptr, framebuffer, 160 * sizeof(uint32_t));
//
//         SDL_RenderCopy(renderer, texture, nullptr, nullptr);
//         SDL_RenderPresent(renderer);
//         SDL_Delay(32);
//     }
//     SDL_DestroyRenderer(renderer);
//     SDL_DestroyWindow(window);
//     SDL_Quit();
//     return 0;
// }

PPU::PPU() {}

void PPU::sdl_init() {
    SDL_Window* win;
    win = SDL_CreateWindow("GameBoy", 0, 0, 160, 144, SDL_WINDOW_SHOWN);
    SDL_Delay(3000);
    SDL_DestroyWindow(win);

    SDL_Quit();
}

void PPU::step(int t_cycle) {}
