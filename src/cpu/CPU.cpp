#include <iostream>
#include <fstream>
#include <vector>
#include "CPU.hpp"



Cpu() {

    pc = 0;
}

std::vector<uint8_t> loadRom(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open ROM file." << std::endl;
        exit(1);
    }

    // Get the size of the file
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);


    if (!file.read(reinterpret_cast<char*>(romData.data()), size)) {
        std::cerr << "Error: Could not read ROM file." << std::endl;
        exit(1);
    }

    file.close();
    return romData;
}

void readOpcodes(const std::vector<uint8_t> &romData) {

    while (pc < romData.size()) {
        opcode = romData[pc];
        pc++; 

        switch (opcode) {
            case 0x00: 

                break;
            case 0x01: 

                break;
            case 0x02:

                break;
            default:

                break;
        }
    }
}
