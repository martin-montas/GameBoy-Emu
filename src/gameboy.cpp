// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
//
#include "gameboy.hpp"
#include "serial.hpp"
#include "timer.hpp"

#include <stdint.h>

GameBoy::GameBoy(std::string filename) {
  serial           = new Serial();
  cpu              = new Cpu();
  timer            = new Timer();
  mmu              = new MMU(filename, timer, serial);
  instructions     = new InstructionSet(mmu, cpu);

  emulationRunning = true;
}

void GameBoy::run() {
  uint8_t _opcode;
  int     cycle_count = 0;
  while (emulationRunning) {
    _opcode           = mmu->romData[cpu->PC];
    int current_cycle = cpu->opcode_cycles[_opcode];
    cpu->cycle_count += current_cycle;

    instructions->execute(_opcode);

    // serial->tick(current_cycle);

    // TODO call timer.tick(curr_cycle) here
    // TODO finish the rest of this loop:
  }
}

uint32_t GameBoy::calculateCyclesForFrame() { return 0; }
