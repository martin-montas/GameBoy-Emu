// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
#ifndef SRC_SST_BUS_HPP_
#define SRC_SST_BUS_HPP_

#include <cstdint>
#include "system-bus.hpp"

class SST : public SystemBus {
  public:
    uint8_t memory[65536]{};

    uint8_t read8(uint16_t addr) override {
        return memory[addr];
    }
    void write8(uint16_t addr, uint8_t value) override {
        memory[addr] = value;
    }

    uint16_t read16(uint16_t addr) override {
        uint8_t low_byte  = read8(addr);
        uint8_t high_byte = read8(addr + 1);
        return (high_byte << 8) | low_byte;
    }
    void write16(uint16_t addr, uint16_t value) override {
        uint8_t low_byte  = value & 0xFF;
        uint8_t high_byte = (value >> 8) & 0xFF;
        write8(addr, low_byte);
        write8(addr + 1, high_byte);
    }

    uint8_t getIE() override {
        return 0;
    }
    uint8_t getIF() override {
        return 0;
    }

    void setIF(uint8_t value) override {
        return;
    }
};
#endif // !SRC_SST_BUS_HPP_
