// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
//
#include <cstdint>
#include <sys/types.h>

#include "cpu/CPU.hpp"
#include "instructions.hpp"

//             TODO:
//
//        Finish the extended methods()
//
void InstructionSet::rlc_extended(uint8_t &reg) {
    uint8_t bit7 = (reg & 0x80) >> 7; 
    reg = (reg << 1);
    reg |= bit7;
    cpu.set_flag(FLAG_CARRY, bit7);
    cpu.set_flag(FLAG_ZERO, reg == 0);
    cpu.clear_flag(FLAG_HALF_CARRY);
    cpu.clear_flag(FLAG_SUBTRACT);
}
void InstructionSet::rcc_extended(uint8_t &reg) {
    bool bit0 = reg & 1;
    cpu.F &= ~FLAG_CARRY;
    cpu.F |= (bit0 << 4);
    reg >>= 1;

    if (bit0) {
        reg |=  0x08;
    }
}

void InstructionSet::rl_extended(uint8_t &reg) {
    //  Suppose reg initially holds the value 0b00001110 (decimal 14) and 
    //  the FLAG_CARRY bit is set. Left Shift: reg becomes 0b00011100 
    //  (decimal 28). 
    //  Carry Flag Check: The expression evaluates to 1.
    //  Bitwise OR: 0b00011100 | 0b00000001 results in 0b00011101 (decimal 29).
    //  Assignment: The value 0b00011101 is assigned back to reg.
    
    bool bit7 = reg & 0x80;
    reg = (reg << 1) | (cpu.F & FLAG_CARRY ? 1 : 0);
    if (bit7) {
        cpu.F |= FLAG_CARRY;    // Sets a bit on the carry flag 
    } else {
        cpu.F &= ~FLAG_CARRY;  // Clears the carry flag
    }
}

void InstructionSet::rr_extended(uint8_t &reg) {
    return;
}

void InstructionSet::sla_extended(uint8_t &reg) {
    return;
}
