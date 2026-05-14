// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
//
#include "mmu.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

MMU::MMU(std::string filename) { load_rom(filename); }

void MMU::load_rom(const std::string &filename) {
  std::ifstream file(filename, std::ios::binary);
  const size_t chunk_size = 1024;
  std::vector<char> chunk(chunk_size);
  while (file.read(chunk.data(), chunk_size)) {
    this->romData.insert(this->romData.end(), chunk.begin(),
                         chunk.begin() + file.gcount());
  }
  // Handle any remaining bytes
  if (file.gcount() > 0) {
    this->romData.insert(this->romData.end(), chunk.begin(),
                         chunk.begin() + file.gcount());
  }
}

uint8_t MMU::read8(uint16_t address) {
  // 0000-7FFF → ROM
  // 8000-9FFF → VRAM
  // A000-BFFF → External RAM (cartridge)
  // C000-DFFF → Work RAM
  // E000-FDFF → Echo RAM (mirror of C000–DDFF)
  // FE00-FE9F → OAM
  // FF00-FF7F → I/O registers
  // FF80-FFFE → HRAM
  // FFFF      → IE register

  if (address <= 0x3FFF) {
    return this->romData[address];

  } else if (address >= 0x8000 && address <= 0x9FFF) {
    return this->VRAM[address - 0x8000];

  } else if (address >= 0xA000 && address <= 0xBFFF) {
    return this->EXTERNAL_RAM[address - 0xA000];

  } else if (address >= 0xC000 && address <= 0xDFFF) {
    return this->WRAM[address - 0xC000];

  } else if (address >= 0xFE00 && address <= 0xFE9F) {
    return this->OAM[address - 0xFE00];

  } else if (address >= 0XFF00 && address <= 0XFF7F) {
    return this->IO_REGISTERS[address - 0XFF00];

  } else if (address >= 0XE000 && address <= 0xFDFF) {
    return this->WRAM[address - 0xE000];

  } else if (address >= 0xFF80 && address <= 0xFFFE) {
    return this->HRAM[address - 0xFF80];
  } else {
    std::cout << "Memory access out of bounds: " << address << std::endl;
    exit(1);
  }
}

void MMU::write8(uint16_t address, uint8_t value) {
  // 0000-7FFF → ROM
  // 8000-9FFF → VRAM
  // A000-BFFF → External RAM (cartridge)
  // C000-DFFF → Work RAM
  // E000-FDFF → Echo RAM (mirror of C000–DDFF)
  // FE00-FE9F → OAM
  // FF00-FF7F → I/O registers
  // FF80-FFFE → HRAM
  // FFFF      → IE register

  if (address >= 0x8000 && address <= 0x9FFF) {
    this->VRAM[address - 0x8000] = value;

  } else if (address >= 0xA000 && address <= 0xBFFF) {
    this->EXTERNAL_RAM[address - 0xA000] = value;

  } else if (address >= 0xC000 && address <= 0xDFFF) {
    this->WRAM[address - 0xC000] = value;

  } else if (address >= 0xFE00 && address <= 0xFE9F) {
    this->OAM[address - 0xFE00] = value;

  } else if (address >= 0XFF00 && address <= 0XFF7F) {
    this->IO_REGISTERS[address - 0XFF00] = value;

  } else if (address >= 0XE000 && address <= 0XFDFF) {
    this->WRAM[address - 0XE000] = value;

  } else if (address >= 0xFF80 && address <= 0xFFFE) {
    this->HRAM[address - 0xFF80] = value;
  } else {
    printf("Memory access out of bounds: %X", address);
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
