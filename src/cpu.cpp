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

Cpu::Cpu(SystemBus* mmu, IInterrupt* interrupt) : _ime(false), _mmu(mmu), _interrupt(interrupt) {
    _instruction = new InstructionSet(mmu, this);
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
    if (this->_ime && _interrupt->pending_interrupt()) {
        _instruction->interrupt_handler();
        return -1;
    }
    uint8_t opcode = _mmu->read8(PC);
    _instruction->execute(opcode);
    int current_cycle = opcode_cycles[opcode];
    return current_cycle;
}
