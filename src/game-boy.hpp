#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include <cstdint>
#include <string>

#include "cpu/CPU.hpp"
#include "memory/MMU.hpp"
#include "instructions.hpp"

class GameBoy {
    private:
        CPU *cpu;
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
