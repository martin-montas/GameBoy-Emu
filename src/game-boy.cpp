
#include <fstream>
#include <iostream>
#include <cstdint>

#include "game-boy.hpp"


GameBoy::~GameBoy() {

}

std::vector<uint8_t> GameBoy::load_rom(const std::string &filename) {
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
