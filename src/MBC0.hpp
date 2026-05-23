#ifndef  SRC_MBC0_HPP_
#define  SRC_MBC0_HPP_

#include <cstdint>

class MBC0: public MBC {
  std::vector<uint8_t>& rom;

public:
  MBC0(std::vector<uint8_t>& rom) : rom(rom) {}

  uint8_t read(uint16_t addr) override {
    if (addr == 0x8000) {
      return rom[addr];
    }
    return 0xFF; 
  }
  void write(uint16_t addr, uint8_t value) override {
    //  finish this:

  }
};

#endif // SRC_MBC0_HPP_
