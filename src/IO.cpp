// copyright 2022 robot locomotion group @ csail. all rights reserved.
// all components of this software are licensed under the gnu license.
// programmer: martin montas, martinmontas0@gmail.com
//

#include "IO.hpp"
#include "timer.hpp"
#include <cstdint>
#include <cstdio>

IO::IO(Timer &timer) { this->timer = timer; }

void IO::write(uint16_t addr, uint8_t value) {
  // reg[addr - 0xFF00] = value;
  switch (addr) {
  case 0xFF01: {
    SB = value;
    break;
  }
  case 0xFF02: {
    char c = reg[0x01];
    printf("%c", c);
    break;
  }
    // DIV (Divider Register)
  case 0xFF04: {
    // this register should be reset if the
    // stop instructIOn executed.
    timer->counter = 0;

    break;
  }
    // TIMA (Time Counter)
  case 0xFF05: {
    timer->TIMA = value;
    break;
  }
    // TMA (IO Modulo)
  case 0xFF06: {
    timer->TMA = value;
    break;
  }
    // TAC (IO Control)
  case 0xFF07: {
    timer->TAC = value;
    break;
  }
  default: {
    break;
  }
  }
}

uint8_t IO::read(uint16_t addr) {
  switch (addr) {
  case 0xFF01: {
    return this->SB;
    break;
  }
  case 0xFF02: {
    char c = reg[0x01];
    printf("%c", c);
    break;
  }
    // DIV (Divider Register)
  case 0xFF04: {
    return static_cast<uint8_t>(this->counter >> 8);
    break;
  }
    // TIMA (Time Counter)
  case 0xFF05: {
    return TIMA;
    break;
  }
    // TMA (IO Modulo)
  case 0xFF06: {
    return TMA;
    break;
  }
    // TAC (IO Control)
  case 0xFF07: {
    return TAC;
    break;
  }
  default: {
    break;
  }
  }
}
