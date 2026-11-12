// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
//
#include <cstdio>
#include <stdint.h>
#include "gameboy.hpp"
#include "cpu.hpp"
#include "joypad.hpp"

void GameBoy::run() {
    _sdl->init();
    // TODO: write the controllers for the joypad with sdl

    // while (SDL_PollEvent(&_sdl->event) && _sdl->display_running) {
    while (_sdl->display_running) {
        while (SDL_PollEvent(&_sdl->event)) {
            switch (_sdl->event.type) {
            case SDL_QUIT:
                _sdl->display_running = false;
                break;

            case SDL_KEYDOWN:
                switch (_sdl->event.key.keysym.sym) {
                case SDLK_x:
                    _joypad->press_button(0);
                    break;

                case SDLK_z:
                    _joypad->press_button(1);
                    break;

                case SDLK_RETURN:
                    _joypad->press_button(3);
                    break;

                case SDLK_RSHIFT:
                    _joypad->press_button(2);
                    break;

                case SDLK_UP:
                    _joypad->press_button(2);
                    break;

                case SDLK_RIGHT:
                    _joypad->press_button(0);
                    break;
                case SDLK_LEFT:
                    _joypad->press_button(1);
                    break;

                case SDLK_DOWN:
                    _joypad->press_button(3);
                    break;
                }
                break;

            case SDL_KEYUP:
                switch (_sdl->event.key.keysym.sym) {
                case SDLK_x:
                    _joypad->release_button(0);
                    break;

                case SDLK_z:
                    _joypad->release_button(1);
                    break;

                case SDLK_RETURN:
                    _joypad->release_button(3);
                    break;

                case SDLK_RSHIFT:
                    _joypad->release_button(2);
                    break;

                case SDLK_UP:
                    _joypad->release_button(2);
                    break;

                case SDLK_RIGHT:
                    _joypad->release_button(0);
                    break;
                case SDLK_LEFT:
                    _joypad->release_button(1);
                    break;

                case SDLK_DOWN:
                    _joypad->release_button(3);
                    break;
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

        if (_cpu->_ime) {
            if (((_interrupt->_IF & _interrupt->_IE) & 0x1F) != 0) {
                _interrupt->exec_handler();
                _cpu->_ime = 0;
            }
        }
    }
}
