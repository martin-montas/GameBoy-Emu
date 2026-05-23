// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
//
#include "mmu.hpp"
#include "IO.hpp"
#include "MBC.hpp"
#include "MBC0.hpp"
#include "timer.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

MMU::MMU(std::string filename, IO *io, Timer *timer) {
  load_rom(filename);
  check_rom_type();
  this->io = io;
  this->timer = timer;
}

void MMU::check_rom_type() {
  uint8_t type = romData[0x0147];
  switch (type) {
  case 0x00:
  case 0x01:
    // More cases should be defined here.
    mbc = std::make_unique<MBC0>(romData);
    break;
  default:
    throw std::runtime_error("Unsupported cartridge");
  }
}

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

uint16_t MMU::read16(uint16_t address) {
  uint8_t low_byte = read8(address);
  uint8_t high_byte = read8(address + 1);
  return (high_byte << 8) | low_byte;
}

uint8_t MMU::read8(uint16_t address) {
  if (address <= 0x7FFF) {
    return mbc->read(address);
  } else if (address >= 0x8000 && address <= 0x9FFF) {
    return this->VRAM[address - 0x8000];
  } else if (address >= 0xA000 && address <= 0xBFFF) {
    // this should also go to mbc
    return this->EXTERNAL_RAM[address - 0xA000];
  } else if ((address >= 0xC000 && address <= 0xDFFF) ||
             (address >= 0xE000 && address <= 0xFDFF)) {
    uint16_t idx = address & 0xC000;
    return this->WRAM[idx];
  } else if (address >= 0xFE00 && address <= 0xFE9F) {
    return this->OAM[address - 0xFE00];
  } else if (address >= 0xFF00 && address <= 0xFF7F) {
    return this->io->read(address - 0xFF00);
  } else if (address >= 0xFF80 && address <= 0xFFFE) {
    return this->HRAM[address - 0xFF80];
  } else {
    std::cout << "Memory access out of bounds: " << address << std::endl;
    exit(1);
  }
}

void MMU::write8(uint16_t address, uint8_t value) {
  if (address >= 0x8000 && address <= 0x9FFF) {
    this->VRAM[address - 0x8000] = value;
  } else if (address >= 0xA000 && address <= 0xBFFF) {
    this->EXTERNAL_RAM[address - 0xA000] = value;
  } else if ((address >= 0xC000 && address <= 0xDFFF) ||
             (address >= 0xE000 && address <= 0xFDFF)) {
    uint16_t idx = address & 0xC000;
    this->WRAM[idx] = value;
  } else if (address == 0xFF04) {
    timer.reset_div();
  } else if (address >= 0xFE00 && address <= 0xFE9F) {
    this->OAM[address - 0xFE00] = value;
  } else if (address >= 0xFF00 && address <= 0xFF7F) {
    this->io->write(address, value);
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
