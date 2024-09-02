#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include "cpu/CPU.hpp"

class GameBoy {
    private:
        CPU *cpu;
        bool emulationRunning;
    public:
        ~GameBoy();
        void run_emulation();
};

#endif // GAMEBOY_HPP
