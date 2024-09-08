#ifndef REGISTERS_HPP
#define REGISTERS_HPP

#include <cstdint>
#include <stdint.h>

class Registers {
    public:
        uint8_t A;
        uint8_t F;
        uint8_t B;
        uint8_t C;
        uint8_t D;
        uint8_t E;
        uint8_t H;
        uint8_t L;

        uint16_t AF;
        uint16_t BC;
        uint16_t DE;
        uint16_t HL;

        uint16_t SP;
        uint16_t PC;

};

#endif // REGISTERS_HPP 
