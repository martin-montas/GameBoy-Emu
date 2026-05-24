// copyright 2022 robot locomotion group @ csail. all rights reserved.
// all components of this software are licensed under the gnu license.
// programmer: martin montas, martinmontas1@gmail.com
//
#ifndef SRC_IO_HPP_
#define SRC_IO_HPP_

#include "MBC.hpp"
#include "timer.hpp"
#include <cstdint>
#include <memory>

enum State { RUNNING, STOPPED, ERROR };

class IO {
public:
  void advance(int cycle);
  void write_div_counter();
  uint8_t read_div_counter();

  IO();

  void set_tac_flag(TAC_TIMER_CONTROL tac_timer);
  uint8_t get_tac_flag();

  uint8_t read(uint16_t addr);
  void write(uint16_t addr, uint8_t value);
  // uint8_t &io_reg(uint16_t addr);
private:
  Timer *timer;
  State state = State::RUNNING;

  TAC_TIMER_CONTROL t_control = TIMER_0;
  std::unique_ptr<MBC> mbc;
  uint8_t reg[0x80] = {};
  uint32_t counter;
};

#endif // SRC_IO_HPP_
