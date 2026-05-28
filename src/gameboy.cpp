// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
//
#include "gameboy.hpp"
#include "serial.hpp"
#include "timer.hpp"

#include <cstdio>
#include <iostream>
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
  while (emulationRunning) {
    if (!cpu->halted) {
      step();
    } else {
      cpu->cycle_count += 4;
    }
  }
}

uint32_t GameBoy::calculateCyclesForFrame() { return 0; }

void     GameBoy::step() {
  _opcode = mmu->romData[cpu->PC];
  instructions->execute(_opcode);
  int current_cycle = cpu->opcode_cycles[_opcode];
  cpu->cycle_count += current_cycle;
  // serial->tick(current_cycle);
  // TODO call timer.tick(curr_cycle) here
  // TODO finish the rest of this loop:
  // timer->tick(current_cycle);
}
