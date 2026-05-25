// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com

#include "serial.hpp"
#include <stdint.h>
#include <stdio.h>

// TODO: write the functionality of _sc bit 0: (0b0000000X)
// Research this:
// Clock select (Read/Write):
// 0 = External clock (“slave”),
// 1 = Internal clock (“master”).
//

void Serial::write_sc(uint8_t value) {
  _sc = value;
  if (_sc >= 0x80) {
    start_transfer()
  } else {
  }
}
void Serial::write_sb(uint8_t value) { _sb = value; }
void Serial::tick(int cycle) {
  _acc += cycle;
  while (_acc >= 512) {
    _acc -= 512;
  }
  if (_sc >= 0x80) {
    _shift_reg = _sb;
    _bit_count = 0;
    shift_one_bit(_shift_reg);
  }
}
void Serial::shift_one_bit(uint8_t &shift_register) {
  uint8_t bit = (_shift_reg & 0x80) ? 1 : 0;
  printf("%d\n", bit);
  _shift_reg <<= 1;
}

uint8_t Serial::read_sb() {}
uint8_t Serial::read_sc() {}
void Serial::start_transfer() {}
