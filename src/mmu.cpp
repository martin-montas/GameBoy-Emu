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
#include <stdio.h>
#include <string>
#include <vector>

MMU::MMU(std::string filename, Timer* timer, Serial* serial) {
    this->timer  = timer;
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

void MMU::load_rom(const std::string& filename) {
    std::ifstream     file(filename, std::ios::binary);
    const size_t      chunk_size = 1024;
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
    uint8_t low_byte  = read8(addr);
    uint8_t high_byte = read8(addr + 1);
    return (high_byte << 8) | low_byte;
}

uint8_t MMU::read8(uint16_t addr) {
    if (addr == 0xD800) {
        // printf("[READ D800] = %02X\n", WRAM[0x0800]);
        return WRAM[addr - 0xC000];
    } else if (addr < 0x8000) {
        return this->romData[addr];
    } else if (addr >= 0x8000 && addr <= 0x9FFF) {
        return VRAM[addr - 0x8000];
    } else if (addr >= 0xA000 && addr <= 0xBFFF) {
        // this should also go to mbc
        return EXTERNAL_RAM[addr - 0xA000];
    } else if (addr >= 0xC000 && addr <= 0xDFFF) {
        return WRAM[addr - 0xC000];
    } else if (addr >= 0xE000 && addr <= 0xFDFF) {
        return WRAM[addr - 0x2000];
    } else if (addr >= 0xFE00 && addr <= 0xFE9F) {
        return OAM[addr - 0xFE00];
    } else if (addr >= 0xFF00 && addr <= 0xFF7F) {
        if (addr >= 0xFF01 && addr <= 0xFF02) {
            return serial->read(addr);
        }
        if (addr >= 0xFF04 && addr <= 0xFF07) {
            return timer->read(addr);
        } else if (addr == 0xFF44) {
            return 0x90;
        } else {
            return IO_REGISTERS[addr - 0xFF00];
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
    if (addr < 0x8000) {
        return;
    } else if (addr >= 0x8000 && addr <= 0x9FFF) {
        VRAM[addr - 0x8000] = value;
        return;
    } else if (addr >= 0xA000 && addr <= 0xBFFF) {
        EXTERNAL_RAM[addr - 0xA000] = value;
        return;
    }
    if (addr >= 0xC000 && addr <= 0xDFFF) {
        if (addr == 0xD800) {
            // printf("[WRITE D800] = %02X \n", value);
        }
        WRAM[addr - 0xC000] = value;
        return;
    } else if (addr >= 0xE000 && addr <= 0xFDFF) {
        WRAM[addr - 0x2000] = value;
        return;
    } else if (addr >= 0xFE00 && addr <= 0xFE9F) {
        OAM[addr - 0xFE00] = value;
        return;
    } else if (addr >= 0xFF00 && addr <= 0xFF7F) {
        if (addr >= 0xFF01 && addr <= 0xFF02) {
            serial->write(addr, value);
            return;
        } else if (addr >= 0xFF04 && addr <= 0xFF07) {
            timer->write(addr, value);
            return;
        } else if (addr >= 0xFEA0 && addr <= 0xFEFF) {
            return; // unused
        } else if (addr == 0xFF44) {
            return;
        } else {
            this->IO_REGISTERS[addr - 0xFF00] = value;
            return;
        }
    } else if (addr >= 0xFF80 && addr <= 0xFFFE) {
        this->HRAM[addr - 0xFF80] = value;
        return;
    } else if (addr == 0xFFFF) {

    } else {
        printf("Memory access out of bounds: %X\n", addr);
        exit(1);
    }
}

void MMU::write16(uint16_t addr, uint16_t value) {
    uint8_t low_byte  = value & 0xFF;
    uint8_t high_byte = (value >> 8) & 0xFF;
    write8(addr, low_byte);
    write8(addr + 1, high_byte);
}
