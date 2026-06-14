// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
//
#include "gameboy.hpp"
#include "serial.hpp"
#include "cpu.hpp"
#include "mmu.hpp"
#include "timer.hpp"
#include "ppu.hpp"

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
        /* main part */
        if (!_cpu->ime_pending) {
            if ((((_interrupt->_IF & _interrupt->_IE) & 0x1F) != 0) && (_cpu->halted)) {
                _cpu->cycle_count += 4;
            } else if (((_interrupt->_IF & _interrupt->_IE) & 0x1F) >= 1) {

            } else if (!_cpu->halted) {
            } else if (_cpu->halted) {
            }
        } else if (_cpu->ime_pending) {
            _cpu->_ime = true;
            continue;
        }
    }
}
