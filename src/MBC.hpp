#ifndef SRC_MBC_HPP_
#define SRC_MBC_HPP_

#include <stdint.h>

class MBC {
public:
  virtual uint8_t read(uint16_t addr) = 0;
  virtual void write(uint16_t addr, uint8_t value) = 0;
};

#endif // SRC_MBC_HPP_
