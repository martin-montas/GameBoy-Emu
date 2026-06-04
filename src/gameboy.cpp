// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
//
#include "gameboy.hpp"
#include "serial.hpp"
#include "cpu.hpp"
#include "mmu.hpp"
#include "timer.hpp"

#include <cstdio>
#include <stdint.h>

GameBoy::GameBoy(const std::string file) {
    cpu              = new Cpu();
    mmu              = new MMU(file);
    instructions     = new InstructionSet(mmu, cpu);
    emulationRunning = true;
    instructions->post_boot_state();
}

void GameBoy::run() {
    while (emulationRunning) {
        if (!cpu->halted) {
            instructions->step();
        } else {
            cpu->cycle_count += 4;
        }
    }
}

uint32_t GameBoy::calculateCyclesForFrame() {
    return 0;
}

void GameBoy::step() {
    _opcode = mmu->read8(cpu->PC);
    instructions->execute(_opcode);
    int current_cycle = cpu->opcode_cycles[_opcode];
    cpu->cycle_count += current_cycle;
}
