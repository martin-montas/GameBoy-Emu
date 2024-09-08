#include <cstdint>

#include "CPU.hpp"
#include "../instructions.hpp"

const double cyclesPerMicrosecond = 4.194304; 
const uint32_t timeSlice = 1000; 

CPU::CPU(Registers *registers, MMU *mmu) {
    this->registers = registers;
    this->mmu = mmu;
    this->instructions = new InstructionSet(registers, mmu);
}

void CPU::step() {
    uint32_t cyclesToRun = timeSlice * cyclesPerMicrosecond;

    while (cyclesToRun > 0) {
        emulate_cycles(cyclesToRun); 
        // cyclesToRun -= cycles; 
    }
}

void CPU::emulate_cycles(uint32_t cyclesToRun) {
        uint8_t opcode;
        while (cyclesToRun > 0) {
            // Fetch and decode opcode
            opcode = mmu->romData[pc];
            pc++;

            // Execute instruction and get cycle count
            uint32_t cycleCount = execute_opcode(opcode);

            // Update global cycle counter
            globalCycles += cycleCount;

            // Decrement cycles to run
            cyclesToRun -= cycleCount;

            // Handle other components (GPU, APU) based on elapsed cycles
            // updateComponents(cycleCount);
        }
}

uint32_t CPU::execute_opcode(uint8_t opcode) {
    uint32_t cycleCount = opcode_cycles[opcode]; 
    opcode_table[opcode]();

    return cycleCount;
}
