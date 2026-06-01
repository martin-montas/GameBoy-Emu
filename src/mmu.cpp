// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
//
#include "mmu.hpp"
// #include "MBC.hpp"
// #include "MBC0.hpp"
#include "serial.hpp"
#include "timer.hpp"

#include <fstream>
#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>

MMU::MMU(std::string filename, Timer *timer, Serial *serial) {
  this->timer = timer;
  this->serial = serial;
  load_rom(filename);
  //  check_rom_type();
}
// void MMU::check_rom_type() {
//   uint8_t type = romData[0x0147];
//   switch (type) {
//   case 0x00:
//   case 0x01:
//     // More cases should be defined here.
//     mbc = std::make_unique<MBC0>(romData);
//     break;
//   default:
//     throw std::runtime_error("Unsupported cartridge");
//   }
// }

void MMU::load_rom(const std::string &filename) {
  std::ifstream file(filename, std::ios::binary);
  const size_t chunk_size = 1024;
  std::vector<char> chunk(chunk_size);
  while (file.read(chunk.data(), chunk_size)) {
    this->romData.insert(this->romData.end(), chunk.begin(), chunk.begin() + file.gcount());
  }
  // Handle any remaining bytes
  if (file.gcount() > 0) {
    this->romData.insert(this->romData.end(), chunk.begin(), chunk.begin() + file.gcount());
  }
}

uint16_t MMU::read16(uint16_t addr) {
  uint8_t low_byte = read8(addr);
  uint8_t high_byte = read8(addr + 1);
  return (high_byte << 8) | low_byte;
}

uint8_t MMU::read8(uint16_t addr) {
  if (addr <= 0x7FFF) {
    return this->romData[addr];
    // }
    // else if (addr <= 0x7FFF) {
    // uint32_t mapped = rom_bank * 0x4000 + (addr - 0x4000);
    // return this->romData[mapped];
  } else if (addr >= 0x8000 && addr <= 0x9FFF) {
    return this->VRAM[addr - 0x8000];
  } else if (addr >= 0xA000 && addr <= 0xBFFF) {
    // this should also go to mbc
    return this->EXTERNAL_RAM[addr - 0xA000];
  } else if (addr >= 0xC000 && addr <= 0xDFFF) {
    return this->WRAM[addr - 0xC000];
  } else if (addr >= 0xE000 && addr <= 0xFDFF) {
    return this->WRAM[addr - 0xE000];
  } else if (addr >= 0xFE00 && addr <= 0xFE9F) {
    return this->OAM[addr - 0xFE00];
  } else if (addr >= 0xFF00 && addr <= 0xFF7F) {
    if (addr >= 0xFF01 && addr <= 0xFF02) {
      return serial->read(addr); // serial register reads
    }
    if (addr >= 0xFF04 && addr <= 0xFF07) {
      return timer->read(addr); // timer registers
    }
    if (addr == 0xFF44) { // stubbed for now
      return 0x90;
    } else {
      return IO_REG[addr - 0xFF00];
    }
  } else if (addr >= 0xFF80 && addr <= 0xFFFE) {
    return this->HRAM[addr - 0xFF80];
  } else {
    printf("Memory access out of bounds: %X\n", addr);
    exit(1);
  }
  return 0xFF;
}

void MMU::write8(uint16_t addr, uint8_t value) {
  if (addr >= 0x0000 && addr <= 0x7FFF) {
    return;
  }
  // if (addr <= 0x1FFF) {
  //   ram_enabled = ((value & 0x0F) == 0x0A);
  //   return;
  // }
  else if (addr >= 0x8000 && addr <= 0x9FFF) {
    this->VRAM[addr - 0x8000] = value;
  } else if (addr >= 0xA000 && addr <= 0xBFFF) {
    this->EXTERNAL_RAM[addr - 0xA000] = value;
  } else if (addr >= 0xC000 && addr <= 0xDFFF) {
    this->WRAM[addr - 0xC000] = value;
  } else if (addr >= 0xE000 && addr <= 0xFDFF) {
    this->WRAM[addr - 0xE000] = value; // echo RAM
  } else if (addr >= 0xFE00 && addr <= 0xFE9F) {
    this->OAM[addr - 0xFE00] = value;
  } else if (addr >= 0xFF00 && addr <= 0xFF7F) {
    if (addr >= 0xFF01 && addr <= 0xFF02) {
      serial->write(addr, value); // serial register reads
      return;
    }
    if (addr >= 0xFF04 && addr <= 0xFF07) {
      timer->write(addr, value);
      return;
    }
    if (addr == 0xFF44) {
      return;
    } else if (addr >= 0xFEA0 && addr <= 0xFEFF) {
      return; // unusable
    }
  } else if (addr >= 0xFF80 && addr <= 0xFFFE) {
    this->HRAM[addr - 0xFF80] = value;
  } else if (addr == 0xFFFF) {
    return;
  } else {
    printf("Memory access out of bounds: %X\n", addr);
    exit(1);
  }
}

void MMU::write16(uint16_t addr, uint16_t value) {
  uint8_t low_byte = value & 0xFF;
  uint8_t high_byte = (value >> 8) & 0xFF;
  write8(addr, low_byte);
  write8(addr + 1, high_byte);
}
