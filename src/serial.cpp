// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com

#include "serial.hpp"
#include <cstdint>
#include <stdint.h>
#include <stdio.h>

void Serial::write_sc(uint8_t value) {
  uint8_t old_sc = _sc;
  _sc = value;

  bool old_start = old_sc & 0x80;
  bool new_start = value & 0x80;
  if (!old_start && new_start) {
    _transfer_active = true;
    _acc = 0;
    _bit_count = 0;
    _shift_reg = _sb;
  }
}
void Serial::write_sb(uint8_t value) { _sb = value; }
void Serial::tick(int cycle) {
  if (_transfer_active) {
    _acc += cycle;
    while (_acc >= 512) {
      _acc -= 512;
      shift_one_bit();
    }
  }
}
void Serial::shift_one_bit() {
  uint8_t bit = (_shift_reg & 0x80) ? 1 : 0;
  printf("%d\n", bit);
  _shift_reg <<= 1;
}

uint8_t Serial::read(uint16_t addr) {
  if (addr == 0xFF01)
    return _sb;
  else
    return _sc;
}

void Serial::write(uint16_t addr, uint8_t value) {
  if (addr == 0xFF01) {
    // TODO:
  } else {
    // TODO:
  }
}
