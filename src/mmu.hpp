// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
//
#ifndef SRC_MEMORY_MMU_HPP_
#define SRC_MEMORY_MMU_HPP_

#include <cstdint>

#include <string>
#include <vector>

class MMU {
 public:
        static std::vector<uint8_t> romData;
        MMU(std::string filename);
        static uint8_t HRAM[0x7F];
        static uint8_t IRAM[8192];

        static uint8_t VRAM[0x2000];
        static uint8_t WRAM[0x2000];
        static uint8_t OAM[0xA0];

        static uint8_t IO_REGISTERS[0x80];
        static uint8_t EXTERNAL_RAM[8192];

        static uint8_t InterruptEnabled;
        static uint8_t read8(uint16_t address);
        static uint16_t read16(uint16_t address);

        static void write8(uint16_t address, uint8_t  value);
        static void write16(uint16_t address, uint16_t value);

        static void load_rom(const std::string &filename);
};

#endif  // SRC_MEMORY_MMU_HPP_
