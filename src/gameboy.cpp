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
        int current_cycle = _cpu->step();
        if (_cpu->ime_pending) {
            _cpu->_ime        = 1;
            _cpu->ime_pending = false;
        }

        if (current_cycle > 0) {
            _timer->tick(current_cycle);
            // _serial->tick(current_cycle);
            _ppu->dot_cycle(current_cycle);
            if (_ppu->can_render) {
                _sdl->frame_step(_ppu->frame_buff);
                _ppu->clear_can_render();
            }
        }

        if (_cpu->halted) {
            if (((_interrupt->_IF & _interrupt->_IE) & 0x1F) != 0) {
                _cpu->halted = false;
            }
        }
    }
}
