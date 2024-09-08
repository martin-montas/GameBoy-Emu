#include <string>
#include <fstream>
#include <iostream>
#include <cstdint>

#include "MMU.hpp"


MMU::MMU(std::string filename) {
    load_rom(filename);

}

void MMU::load_rom(const std::string &filename) {
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
}


uint8_t MMU::read(uint16_t address) {

    if ( address >= 0x8000 && address <= 0x9FFF ){
        return VRAM[address - 0x8000];
    }
    else if ( address >= 0xA000 && address <= 0xBFFF ){
        return EXTERNAL_RAM[address - 0xA000];
    }
    else if ( address >= 0xC000 && address <= 0xDFFF ){
        return WRAM[address - 0xC000];
    }
    else if ( address >= 0xFE00 && address <= 0xFE9F ){
        return OAM[address - 0xFE00];
    }
    else if ( address >= 0XFF00 && address <= 0XFF7F ){
        return IO_REGISTERS[address - 0XFF00];
    }
    else if ( address >= 0XE000 && address <= 0XFDFF ){
        return WRAM[address - 0X2000];
    }
    else if (address >= 0xFF80 && address <= 0xFFFE) {
        return HRAM[address - 0xFF80];
    }
    else {
        std::cout << "Memory access out of bounds: "  << address << std::endl;
        exit(1);
    }
}
