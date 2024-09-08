#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include <string>

#include "cpu/CPU.hpp"
#include "memory/MMU.hpp"
#include "registers.hpp"
#include "instructions.hpp"

class GameBoy {
    private:
        CPU *cpu;
        MMU *mmu;

        Registers *registers;
        InstructionSet *instructions;

        bool emulationRunning;
    public:
        ~GameBoy();
        void run();
        void init(std::string filename);
};

#endif // GAMEBOY_HPP
