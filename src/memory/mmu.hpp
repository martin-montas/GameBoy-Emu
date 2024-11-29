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
        std::vector<uint8_t> romData;
        explicit MMU(std::string filename);
        ~MMU();
        uint8_t HRAM[0x7F];
        uint8_t IRAM[8192];

        uint8_t VRAM[0x2000];
        uint8_t WRAM[0x2000];
        uint8_t OAM[0xA0];

        uint8_t IO_REGISTERS[0x80];
        uint8_t EXTERNAL_RAM[8192];

        uint8_t InterruptEnabled;
        uint8_t read8(uint16_t address);
        uint16_t read16(uint16_t address);

        void write8(uint16_t address, uint8_t  value);
        void write16(uint16_t address, uint16_t value);

        void load_rom(const std::string &filename);
};

#endif  // SRC_MEMORY_MMU_HPP_
