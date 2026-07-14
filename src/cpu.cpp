// Copyright 2023 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
//
#include "cpu.hpp"

#include <stdint.h>
#include <vector>

const double   cyclesPerMicrosecond = 4.194304;
const uint32_t timeSlice            = 1000;
uint32_t       cyclesToRun          = timeSlice * cyclesPerMicrosecond;

Cpu::Cpu(SystemBus* bus, IInterrupt* interrupt) : _ime(0), _bus(bus), _interrupt(interrupt) {
    _instruction = new InstructionSet(bus, this);
}
bool Cpu::is_flag_set(uint8_t flag) {
    return this->F & flag;
}

void Cpu::clear_flag(uint8_t flag) {
    this->F &= ~flag;
}

void Cpu::set_flag(uint8_t flags, bool state) {
    this->F = state ? (this->F | flags) : (this->F & ~flags);
}

int Cpu::step() {
    if (halted) {
        return 4;
    }
    uint8_t opcode = _bus->read8(PC);
    _instruction->execute(opcode);
    int current_cycle = opcode_cycles[opcode];
    return current_cycle;
}
