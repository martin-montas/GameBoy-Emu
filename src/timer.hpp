// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
//
#ifndef SRC_TIMER_HPP_
#define SRC_TIMER_HPP_

#include <cstdint>

#define TAC_SELECT 0b11

enum TAC_TIMER_CONTROL : uint8_t {
  TIMER_0 = 0b00,
  TIMER_1 = 0b01,
  TIMER_2 = 0b10,
  TIMER_3 = 0b11
};

class Timer {
private:
  uint8_t SB;
  uint32_t DIV;
  uint8_t SC;
  uint8_t TIMA;
  uint8_t TMA;
  uint8_t TAC;
  uint16_t internal_div_counter;

  int curr_frequency = 256;

public:
  void tick(int cycle);
  uint8_t get_tac_flag();
  void set_tac_flag(TAC_TIMER_CONTROL tac_timer);
  int time_frequency_select();
};
#endif // !SRC_TIMER_HPP_
