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
    while (emulationRunning) {
        if (!_cpu->halted) {
            step();
        } else {
            _cpu->cycle_count += 4;
        }
    }
}

uint32_t GameBoy::calculateCyclesForFrame() {
    return 0;
}

void GameBoy::step() {
    _opcode = _mmu->read8(_cpu->PC);
    _instructions->execute(_opcode);
    int current_cycle = _cpu->opcode_cycles[_opcode];
    _cpu->cycle_count += current_cycle;
    _ppu->dot_cycle(current_cycle);
}
