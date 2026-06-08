#include <iostream>
#include <cstdint>
#include <SDL2/SDL.h>
#include "ppu.hpp"

int main() {

    PPU* ppu = new PPU();
    ppu->run();
    delete (ppu);
}
