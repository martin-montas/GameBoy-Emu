#include "registers.h"
#include <stdint>

void Registers::HL(uint8_t &h, uint8_t &l) {

    uint16_t HL = (H << 8) | L;
}

void Registers::AF(uint8_t &a, uint8_t &f) {

    uint16_t AF = (a << 8) | f;
}


void Registers::BC(uint8_t &b, uint8_t &c) {

    uint16_t BC = (b << 8) | c;
}

void Registers::DE(uint8_t &d, uint8_t &e) {

    uint16_t DE = (d << 8) | e;
}

