// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
//
#include <cstdint>
#include "game-boy.hpp"

void GameBoy::init(std::string filename) {
    mmu = new MMU(filename);
    instructions = new InstructionSet(cpu, mmu);
    emulationRunning = true;
}

GameBoy::~GameBoy() {}
void GameBoy::run() {
    uint8_t opcode;
    int cycle_count = 0;
    while (emulationRunning) {
        opcode = mmu->romData[cpu->PC];

        instructions->execute(opcode);
        int current_cycle = cpu->opcode_cycles[opcode];

        cpu->cycle_count += current_cycle;
        // Render frame, update audio, etc.
    }
}

uint32_t GameBoy::calculateCyclesForFrame() {
    return 0;
}
