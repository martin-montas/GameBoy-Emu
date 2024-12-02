// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com

#ifndef SRC_GAME_BOY_HPP_
#define SRC_GAME_BOY_HPP_

#include <cstdint>
#include <string>

#include "cpu.hpp"
#include "mmu.hpp"
#include "instructions.hpp"

class GameBoy {
 private:
        Cpu *cpu;
        MMU *mmu;
        InstructionSet *instructions;
        bool emulationRunning;

 public:
        GameBoy(std::string filename);
        ~GameBoy();
        void run();
        uint32_t calculateCyclesForFrame();
};

#endif  // SRC_GAME_BOY_HPP_
