// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
//
#include "gameboy.hpp"
#include "cpu.hpp"

#include <cstdio>
#include <stdint.h>

void GameBoy::run() {
    _sdl->init();
    while (_sdl->display_running) {
        while (SDL_PollEvent(&_sdl->event)) {
            switch (_sdl->event.type) {
            case SDL_QUIT:
                _sdl->display_running = false;
                break;
            case SDL_KEYDOWN:
                if (_sdl->event.key.keysym.sym == SDLK_ESCAPE) {
                    _sdl->display_running = false;
                    return;
                }
                break;
            }
        }
        /* main emu part */
        if (_cpu->ime_pending) {
            _cpu->_ime        = 1;
            _cpu->ime_pending = false;
            _cpu->step();
        } else {
            if (((_interrupt->_IF & _interrupt->_IE) & 0x1F) != 0) {
                _cpu->halted = false;
                continue;
            } else if (((_interrupt->_IF & _interrupt->_IE) & 0x1F) == 0) {
                _cpu->cycle_count += 4;
                continue;
            }
        }
        _cpu->step();
    }
}
