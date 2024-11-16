// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
//
#include <cstdint>
#include <iostream>
#include "CPU.hpp"

const double cyclesPerMicrosecond = 4.194304; 
const uint32_t timeSlice = 1000; 
uint32_t cyclesToRun = timeSlice * cyclesPerMicrosecond;

CPU::CPU(MMU &mmu): mmu(mmu) {}

bool CPU::is_flag_set(uint8_t flag) {
    return this->F & flag;
}

void CPU::clear_flag(uint8_t flag) {
    this->F &= ~flag;
}

void CPU::set_flag(uint8_t flags, bool state) {
    this->F = state ? (this->F | flags) : (this->F & ~flags);
}

void CPU::print_flags(){
    std::cout << "Z: " << std::hex << +is_flag_set(FLAG_ZERO) << " N: " << std::hex << +is_flag_set(FLAG_SUBTRACT) << " H: " << std::hex << +is_flag_set(FLAG_HALF_CARRY) << " C: " << std::hex << +is_flag_set(FLAG_CARRY) << std::endl;
}
