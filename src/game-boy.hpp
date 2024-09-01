#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include <SFML/Graphics.hpp>

#include "cpu/CPU.hpp"

class GameBoy {

    private:
        CPU *cpu;

    public:
        GameBoy();
        ~GameBoy();

};

#endif // GAMEBOY_HPP;
