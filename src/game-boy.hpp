#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include "cpu/CPU.hpp"

class GameBoy {

    private:
        CPU *cpu;

      std::vector<uint8_t> romData;
    public:
        // GameBoy();
        ~GameBoy();
        std::vector<uint8_t> load_rom(const std::string &filename);

};

#endif // GAMEBOY_HPP
