#ifndef SRC_IO_HPP_
#define SRC_IO_HPP_

#include <cstdint>

class IO {
private:
  uint8_t SB; // for debugging
  uint8_t SC; // for debugging
public:
  uint8_t read(uint16_t addr);
  void    write(uint16_t addr, uint8_t value);
};

#endif // SRC_IO_HPP_
