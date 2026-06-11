// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#include "sdl-utils.hpp"
#include <SDL2/SDL.h>
#include <iostream>

void SDL::init() {
    display_running = true;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL Error: %s\n", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow("SDL TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH * 2,
                              HEIGHT * 2, SDL_WINDOW_SHOWN);

    SDL_StopTextInput();
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
}

SDL::~SDL() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void SDL::frame_step(uint32_t* frame_buff) {
    SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);
    SDL_EventState(SDL_KEYUP, SDL_ENABLE);

    // while (SDL_PollEvent(&event)) {
    //     switch (event.type) {
    //     case SDL_QUIT:
    //         display_running = false;
    //         break;
    //     case SDL_KEYDOWN:
    //         std::cout << "it went through\n";
    //         if (event.key.keysym.sym == SDLK_ESCAPE) {
    //             display_running = false;
    //             return;
    //         }
    //         break;
    //     }
    // }
    //
    SDL_UpdateTexture(texture, nullptr, frame_buff, WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}
