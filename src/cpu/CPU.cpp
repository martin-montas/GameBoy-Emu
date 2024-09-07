#include <cstdint>
#include <fstream>
#include <iostream>

#include "CPU.hpp"

const double cyclesPerMicrosecond = 4.194304; 
const uint32_t timeSlice = 1000; 

CPU::CPU(Registers *registers, MMU *mmu,  InstructionSet *instructions) {
    this->registers = registers;
    this->mmu = mmu;
    this->instructions = instructions;
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
            opcode = romData[pc];
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

std::vector<uint8_t> CPU::load_rom(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open ROM file." << std::endl;
        exit(1);
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (!file.read(reinterpret_cast<char*>(romData.data()), size)) {
        std::cerr << "Error: Could not read ROM file." << std::endl;
        exit(1);
    }

    file.close();
    return romData;
}


