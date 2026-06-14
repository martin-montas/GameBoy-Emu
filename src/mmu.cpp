// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
//
#include "mmu.hpp"
// #include "MBC.hpp"
// #include "MBC0.hpp"
#include "serial.hpp"
#include "ppu.hpp"
#include "timer.hpp"

#include <fstream>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <vector>

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

void Mmu::load_rom(const std::string& filename) {
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

uint16_t Mmu::read16(uint16_t addr) {
    uint8_t low_byte  = read8(addr);
    uint8_t high_byte = read8(addr + 1);
    return (high_byte << 8) | low_byte;
}

uint8_t Mmu::read8(uint16_t addr) {
    // 0000-00FF  Boot ROM (while enabled)
    // 0100-3FFF  ROM bank 0
    // 4000-7FFF  ROM bank n
    // 8000-9FFF  VRAM
    // A000-BFFF  Cartridge RAM
    // C000-CFFF  WRAM bank 0
    // D000-DFFF  WRAM bank 1
    // E000-FDFF  Echo RAM
    // FE00-FE9F  OAM
    // FEA0-FEFF  Unusable
    // FF00-FF7F  I/O registers
    // FF80-FFFE  HRAM
    // FFFF       IE register

    if (addr == 0xD800) {
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
    } else if (addr >= 0xFEA0 && addr <= 0xFEFF) {
        return 0xFF;
    } else if (addr >= 0xFF00 && addr <= 0xFF7F) {
        if (addr >= 0xFF01 && addr <= 0xFF02) {
            return serial.read(addr);
        } else if (addr >= 0xFF04 && addr <= 0xFF07) {
            return _timer->read(addr);
        } else if (addr == 0xFF0F) {
            printf("interurpt read by 0xFF0F\n");
            return _interrupt->read(addr);
        } else if (addr == 0xFF44) {
            return _ppu->read_ly();
        } else {
            return IO_REGISTERS[addr - 0xFF00];
        }

    } else if (addr >= 0xFF80 && addr <= 0xFFFE) {
        return this->HRAM[addr - 0xFF80];
    } else if (addr == 0xFFFF) {
        printf("interurpt read by 0xFFFF\n");
        return _interrupt->read(addr);
    } else {
        printf("Readable memory access out of bounds: %x\n", addr);
        // exit(1);
    }
    return 0xFF;
}

void Mmu::write8(uint16_t addr, uint8_t value) {
    // 0000-00FF  Boot ROM (while enabled)
    // 0100-3FFF  ROM bank 0
    // 4000-7FFF  ROM bank n
    // 8000-9FFF  VRAM
    // A000-BFFF  Cartridge RAM
    // C000-CFFF  WRAM bank 0
    // D000-DFFF  WRAM bank 1
    // E000-FDFF  Echo RAM
    // FE00-FE9F  OAM
    // FEA0-FEFF  Unusable
    // FF00-FF7F  I/O registers
    // FF80-FFFE  HRAM
    // FFFF       IE register

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
    } else if (addr >= 0xFEA0 && addr <= 0xFEFF) {
        return; // unused
    } else if (addr >= 0xFF00 && addr <= 0xFF7F) {
        if (addr >= 0xFF01 && addr <= 0xFF02) {
            serial.write(addr, value);
            return;
        } else if (addr >= 0xFF04 && addr <= 0xFF07) {
            _timer->write(addr, value);
            return;
        } else if (addr == 0xFF0F) {
            printf("interrupt write to %X by 0xFF0F\n", value);
            _interrupt->write(addr, value);
            return;
        } else if (addr == 0xFF44) {
            _ppu->write_ly(value);
            return;
        } else {
            this->IO_REGISTERS[addr - 0xFF00] = value;
            return;
        }
    } else if (addr >= 0xFF80 && addr <= 0xFFFE) {
        this->HRAM[addr - 0xFF80] = value;
        return;
    } else if (addr == 0xFFFF) {
        printf("interrupt write to %X by 0xFFFF\n", value);
        _interrupt->write(addr, value);
        return;
    } else {
        printf("Writable memory access out of bounds: %X\n", addr);
        // exit(1);
    }
}

void Mmu::write16(uint16_t addr, uint16_t value) {
    uint8_t low_byte  = value & 0xFF;
    uint8_t high_byte = (value >> 8) & 0xFF;
    write8(addr, low_byte);
    write8(addr + 1, high_byte);
}
