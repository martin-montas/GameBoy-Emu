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

        if ((cpu->_ime) &&)

            _opcode = _mmu->read8(_cpu->PC);
        if (_cpu->ime_pending) {

            _instructions->execute(_opcode);
            int current_cycle = _cpu->opcode_cycles[_opcode];

            _cpu->cycle_count += current_cycle;
            _ppu->dot_cycle(current_cycle);
            if (_ppu->can_render) {
                _sdl->frame_step(_ppu->frame_buff);
                _ppu->clear_can_render();
                // handle interrupts here.
            }
        } else {
            if (_cpu->halted) {
                // check for interrupts here
                _cpu->ime_pending = false;
                _cpu->cycle_count += 4;
            }
        }
    }
}

void GameBoy::step() {}
