#include "cpu/CPU.hpp"
#include "instructions.hpp"
#include <cstdint>
#include <sys/types.h>

void InstructionSet::rlc_extended(uint8_t reg) {
     uint8_t bit7 = (reg & 0x80) >> 7; 
     reg = (reg << 1);
     reg |= bit7;

     cpu.set_flag(FLAG_CARRY, bit7);
     cpu.set_flag(FLAG_ZERO, reg == 0);
     cpu.clear_flag(FLAG_HALF_CARRY);
     cpu.clear_flag(FLAG_SUBTRACT);
}

