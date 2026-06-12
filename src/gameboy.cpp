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

void GameBoy::unhalt() {}

void GameBoy::interrupt_handler() {
    if (((_interrupt->_IF & _interrupt->_IE) & 0x1F) != 0) {
        /* check for interrupts here */
        _cpu->SP -= 1;
        uint8_t h = _cpu->PC >> 8;
        _mmu->write8(_cpu->SP, h);
        _cpu->SP -= 1;
        uint8_t l = _cpu->PC & 0xFF;
        _mmu->write8(_cpu->SP, l);
        uint8_t vec = _interrupt->get_interrupt_vector();
        _cpu->PC    = vec;
        _cpu->cycle_count += 20;
    }
}

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
        if (!_cpu->ime_pending) {
            if (((_interrupt->_IF & _interrupt->_IE) & 0x1f) != 0) {
                _cpu->halted = false;
                if (_cpu->_ime) {
                    interrupt_handler();
                } else {
                    step();
                    continue;
                }
            } else {
                if (_cpu->halted) {
                    _cpu->cycle_count += 4;
                } else {
                    step();
                    continue;
                }
            }
        } else {
            _cpu->ime_pending = false;
            _cpu->_ime        = 1;
        }
    }
}

void GameBoy::step() {
    _opcode = _mmu->read8(_cpu->PC);
    _instructions->execute(_opcode);
    int current_cycle = _cpu->opcode_cycles[_opcode];
    _cpu->cycle_count += current_cycle;
    _ppu->dot_cycle(current_cycle);
    _timer->tick(current_cycle);
    if (_ppu->can_render) {
        _sdl->frame_step(_ppu->frame_buff);
        _ppu->clear_can_render();
    }
}
