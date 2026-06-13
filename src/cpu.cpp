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

Cpu::Cpu(Ppu* ppu, Timer* timer, SDL* sdl, Mmu* mmu, IInterrupt* interrupt)
    : _ppu(ppu), _timer(timer), _sdl(sdl), _mmu(mmu), _interrupt(interrupt) {
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

void Cpu::step(uint8_t cycle_count) {

    if (this->_ime && _interrupt->pending_interrupt()) {
        _instruction->interrupt_handler();
        return; // Skip fetching normal opcode this cycle
    }

    _instruction->execute(cycle_count);
    int current_cycle = opcode_cycles[cycle_count];
    cycle_count += current_cycle;
    _ppu->dot_cycle(current_cycle);
    _timer->tick(current_cycle);

    if (_ppu->can_render) {
        _sdl->frame_step(_ppu->frame_buff);
        _ppu->clear_can_render();
    }
}
