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
void InstructionSet::rlc_extended(uint8_t reg) {
     uint8_t bit7 = (reg & 0x80) >> 7; 
     reg = (reg << 1);
     reg |= bit7;

     cpu.set_flag(FLAG_CARRY, bit7);
     cpu.set_flag(FLAG_ZERO, reg == 0);
     cpu.clear_flag(FLAG_HALF_CARRY);
     cpu.clear_flag(FLAG_SUBTRACT);
}
void InstructionSet::rcc_extended(uint8_t reg) {
     bool bit0 = reg & 1;
     cpu.F &= ~FLAG_CARRY;
     cpu.F |= (bit0 << 4);
     reg >>= 1;

     if (bit0) {
          reg |=  0x08;
    }
}
