// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
//
#include "timer.hpp"
#include "interface-interrupt.hpp"
#include <stddef.h>
#include <stdint.h>
// Timer::Timer() {}
int Timer::clock_speed() {
    switch (_tac & 0b11) {
    case 0: {
        return curr_frequency = 1024;
        break;
    }
    case 1: {
        return curr_frequency = 16;
        break;
    }
    case 2: {
        return curr_frequency = 64;
        break;
    }
    case 3: {
        return curr_frequency = 256;
        break;
    }
    }
    return -1;
}

void Timer::write(uint16_t addr, uint8_t value) {
    switch (addr) {
    case 0xFF04:
        /*
         * Writing any value to DIV resets it.
         */
        _div         = 0;
        _div_counter = 0;
        break;

    case 0xFF05:
        _tima = value;
        break;

    case 0xFF06:
        _tma = value;
        break;

    case 0xFF07:
        /*
         * Only lower 3 bits are used.
         */
        _tac = value & 0x07;
        break;
    }
}
uint8_t Timer::read(uint16_t addr) {
    if (addr == 0xFF04) {
        return _div;
    }
    if (addr == 0xFF05) {
        return _tima;
    }
    if (addr == 0xFF06) {
        return _tma;
    }
    if (addr == 0xFF07) {
        return _tac;
    }
    return 0x00;
}

void Timer::tick(int cycle) {
    // TODO: finish the timer here:
    int threshold = clock_speed();
    _acc += cycle;

    while (_acc >= threshold) {
        _acc -= threshold;
        _div_counter += 1;
    }
    if (_tac & 0b100) {
        if (threshold != -1) {
            while (_tima >= threshold) {
                _tima -= threshold;
                _tima = _tima + 1;
                if (_tima == 0x00) {
                    _tima = _tma;
                }
                if (_tima == 0xFF) {
                    _tima = _tima + 1;
                    _interrupt->request_interrupt(INTERRUPT_TIMER);
                }
            }
        }
    }
}
