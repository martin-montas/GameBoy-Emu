#include <cstdint>
#include <iostream>
#include <fstream>

#include "CPU.hpp"

CPU::CPU() {
    init_opcode_table();
}

void CPU::init_opcode_table() {
    opcode_table[0x00] = [this]() { nop(); };
    opcode_table[0x01] = [this]() { load_bc_nn(); };
    opcode_table[0x02] = [this]() { load_bc_a(); };
}

void CPU::nop() {
    // Handle NOP (No Operation) instruction
    return;
}

void CPU::load_bc_nn() {
    // Handle LD BC,nn instruction
    return;
}

void CPU::load_bc_a() {
    // Handle LD (BC),A instruction
    return;
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

void CPU::read_rom(const std::vector<uint8_t> &romData) {
    uint8_t opcode;
    while (pc < romData.size()) {
        opcode = romData[pc];
        pc++; 

        opcode_table[opcode]();
    }
}
