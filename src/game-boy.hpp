// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
//
#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include <cstdint>
#include <string>

#include "cpu/cpu.hpp"
#include "memory/MMU.hpp"
#include "instructions.hpp"

class GameBoy {
 private:
        Cpu *cpu;
        MMU *mmu;
        InstructionSet *instructions;

        bool emulationRunning;

 public:
        ~GameBoy();
        void init(std::string filename);
        void run();
        uint32_t calculateCyclesForFrame();
};

#endif // GAMEBOY_HPP
