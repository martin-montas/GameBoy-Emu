// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
//
#include "timer.hpp"
#include <cstddef>
#include <cstdint>
// Timer::Timer() {}

Timer::Timer()
    : _sb(0), _div(0), _sc(0), _tima(0), _tma(0), _tac(0), _tima_accumulator(0),
      _div_counter(0) {}

uint8_t Timer::get_tac_flag() { return _tac & TAC_SELECT; }

int Timer::get_time_frequency() {
  switch ((_tac) & 0b11) {
  case 0x00: {
    return curr_frequency = 1024;
    break;
  }
  case 0x01: {
    return curr_frequency = 16;
    break;
  }
  case 0x02: {
    return curr_frequency = 64;
    break;
  }
  case 0x03: {
    return curr_frequency = 256;
    break;
  }
  }
  return -1;
}

void Timer::write(uint16_t addr, uint8_t value) {
  if (addr == 0xFF04) {
    /*
     * @brief: writing to the div timer register
     * disables it.
     */
    _div = 0;
  }
  if (addr == 0xFF05) {
    _tima = value;
  }
  if (addr == 0xFF06) {
    _tma = value;
  }
  if (addr == 0xFF07) {
    // TAC = 0b00000101
    //         ||||||||
    //         |||||||+-- clock bit 0
    //         ||||||+--- clock bit 1
    //         |||||+---- enable
    //
    _tac = value;
  }
}

uint8_t Timer::read(uint16_t addr) {
  if (addr == 0xFF05) {
    return _tima;
  }
  if (addr == 0xFF06) {
    return _tma;
  }
  if (addr == 0xFF07) {
    return _tac;
  }
  return 0x00;
}

uint16_t Timer::read_div(uint16_t addr) {
  if (addr == 0xFF04) {
    return _div;
  }
}

void Timer::tick(int cycle) {
  _div_counter += cycle;
  bool timer_enabled = _tac & 0b100;
  if (timer_enabled) {
    _tima_accumulator += cycle;
    int threshold = this->get_time_frequency();
    while (_tima_accumulator >= threshold) {
      _tima_accumulator -= threshold;
      _tima = _tima + 1;
      if (_tima == 0x00) {
        _tima = _tma;
        // TODO THIS:
        // request_interrupt()
      }
    }
  }
}
