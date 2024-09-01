#include "registers.hpp"
#include <stdint.h>

void Registers::HL(uint8_t &h, uint8_t &l) {

    uint16_t hl = (h << 8) | l;
}

void Registers::AF(uint8_t &a, uint8_t &f) {

    uint16_t af = (a << 8) | f;
}

void Registers::BC(uint8_t &b, uint8_t &c) {

    uint16_t bc = (b << 8) | c;
}

void Registers::DE(uint8_t &d, uint8_t &e) {

    uint16_t de = (d << 8) | e;
}

