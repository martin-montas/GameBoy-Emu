#ifndef SRC_SERIAL_HPP_
#define SRC_SERIAL_HPP_

#include <stdint.h>

class Serial {
  uint8_t _sb = 0x00;
  uint8_t _sc = 0x7E;

private:
  void writeSB(uint8_t value);
  void writeSC(uint8_t value);

  uint8_t readSB();
  uint8_t readSC();

  void tick();

public:
};

#endif // SRC_SERIAL_HPP_
