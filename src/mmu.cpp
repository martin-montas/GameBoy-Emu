// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
//
#include <fstream>
#include <iostream>
#include <cstdint>

#include <string>
#include "mmu.hpp"

MMU::MMU(std::string filename) {
    load_rom(filename);
}

void MMU::load_rom(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    const size_t chunk_size = 1024;
    std::vector<char> chunk(chunk_size);
    while (file.read(chunk.data(), chunk_size)) {
        romData.insert(romData.end(), chunk.begin(), chunk.begin() + file.gcount());
    }
    // Handle any remaining bytes
    if (file.gcount() > 0) {
        romData.insert(romData.end(), chunk.begin(), chunk.begin() + file.gcount());
    }
}


uint8_t MMU::read8(uint16_t address) {
    if (address >= 0x8000 && address <= 0x9FFF) {
        return VRAM[address - 0x8000];
    } else if (address >= 0xA000 && address <= 0xBFFF) {
        return EXTERNAL_RAM[address - 0xA000];
    } else if (address >= 0xC000 && address <= 0xDFFF) {
        return WRAM[address - 0xC000];
    } else if (address >= 0xFE00 && address <= 0xFE9F) {
        return OAM[address - 0xFE00];
    } else if (address >= 0XFF00 && address <= 0XFF7F) {
        return IO_REGISTERS[address - 0XFF00];
    } else if (address >= 0XE000 && address <= 0XFDFF) {
        return WRAM[address - 0X2000];
    } else if (address >= 0xFF80 && address <= 0xFFFE) {
        return HRAM[address - 0xFF80];
    } else {
        std::cout << "Memory access out of bounds: "  << address << std::endl;
        exit(1);
    }
}

void MMU::write8(uint16_t address, uint8_t value) {
    if (address>= 0x8000 && address <= 0x9FFF) {
        VRAM[address - 0x8000] = value;
    } else if (address >= 0xA000 && address <= 0xBFFF) {
        EXTERNAL_RAM[address - 0xA000] = value;
    } else if (address >= 0xC000 && address <= 0xDFFF) {
        WRAM[address - 0xC000] = value;
    } else if (address >= 0xFE00 && address <= 0xFE9F) {
        OAM[address - 0xFE00] = value;
    } else if (address >= 0XFF00 && address <= 0XFF7F) {
        IO_REGISTERS[address - 0XFF00] = value;
    } else if (address >= 0XE000 && address <= 0XFDFF) {
        WRAM[address - 0XE000] = value;
    } else if (address>= 0xFF80 && address <= 0xFFFE) {
        HRAM[address - 0xFF80] = value;
    } else {
        std::cout << "Memory access out of bounds: "  << address << std::endl;
        exit(1);
    }
}

void MMU::write16(uint16_t address, uint16_t value) {
    uint8_t low_byte = value & 0xFF;
    uint8_t high_byte = (value >> 8) & 0xFF;

    write8(address, low_byte);
    write8(address + 1, high_byte);
}

uint16_t MMU::read16(uint16_t address) {
    uint8_t low_byte = read8(address);
    uint8_t high_byte = read8(address + 1);
    return (high_byte << 8) | low_byte;
}

