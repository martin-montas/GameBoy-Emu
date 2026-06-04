#ifndef SRC_SST_BUS_HPP_
#define SRC_SST_BUS_HPP_

#include <cstdint>
#include "system-bus.hpp"

class SST : public SystemBus {
    // still not implemented
  public:
    uint8_t memory[65536]{};

    uint8_t read8(uint16_t addr) override {
        return memory[addr];
    }
    void write8(uint16_t addr, uint8_t value) override {
        memory[addr] = value;
    }

    uint16_t read16(uint16_t addr) override {
        return 0;
    }
    void write16(uint16_t addr, uint16_t value) override {
        return;
    }
};
#endif // !SRC_SST_BUS_HPP_
