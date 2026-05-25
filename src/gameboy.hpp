// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
#ifndef SRC_GAME_BOY_HPP_
#define SRC_GAME_BOY_HPP_

#include <stdint.h>
#include <string>

#include "cpu.hpp"
#include "instructions.hpp"
#include "mmu.hpp"
#include "serial.hpp"
#include "timer.hpp"

using namespace std;
class GameBoy {
private:
  Cpu *cpu;
  MMU *mmu;
  Timer *timer;
  Serial *serial;

  InstructionSet *instructions;
  bool emulationRunning;

public:
  GameBoy(std::string filename);
  uint32_t calculateCyclesForFrame();
  void run();
};

#endif // SRC_GAME_BOY_HPP_
