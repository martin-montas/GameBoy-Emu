
// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
//
#include "game-boy.hpp"
#include "IO.hpp"

#include <cstdint>

GameBoy::GameBoy(std::string filename) {
  cpu = new Cpu();
  timer = new Timer();
  io = new IO();
  mmu = new MMU(filename, io, timer);
  instructions = new InstructionSet(mmu, cpu);

  emulationRunning = true;
}

GameBoy::~GameBoy() {}

void GameBoy::run() {
  uint8_t _opcode;
  int cycle_count = 0;
  while (emulationRunning) {
    _opcode = mmu->romData[cpu->PC];

    instructions->execute(_opcode);
    int current_cycle = cpu->opcode_cycles[_opcode];
    // cpu->cycle_count += current_cycle;
    // TODO: make this a another object
    io->advance(cpu->opcode_cycles[_opcode]);
    // TODO finish the rest of this loop:
    // Render frame, update audio, etc.
  }
}

uint32_t GameBoy::calculateCyclesForFrame() { return 0; }
