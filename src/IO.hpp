#ifndef SRC_IO_HPP_
#define SRC_IO_HPP_

#include <cstdint>

class IO {
public:
  uint8_t read(uint16_t addr);
  void    write(uint16_t addr, uint8_t value);

private:
};

#endif // SRC_IO_HPP_
