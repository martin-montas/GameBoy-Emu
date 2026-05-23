#include "timer.hpp"
#include <cstdint>

int increment = 256;
int selected_frequency = 4096;
int t_counter = 0;
int curr_frequency = 256;

Timer::Timer() {}

// TODO research tac flag
// void Timer::set_tac_flag(TAC_TIMER_CONTROL tac_timer) {
//   this->TAC |= tac_timer;tick
// }
uint8_t Timer::get_tac_flag() { return this->TAC & TAC_SELECT }

int Timer::time_frequency_select() {
  switch ((this->TAC) & 0b11) {
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
}
void Timer::reset_div() { internal_div_counter = 0; }

void Timer::write_tac() {}

void Timer::tick(int cycle) {
  internal_div_counter += cycle;
  bool timer_enabled = TAC & 0b100;
  if (timer_enabled) {
    tima_accumulator += cycle;
    int threshold = this->get_time_frequency();
    while (tima_accumulator >= threshold) {
      tima_accumulator -= threshold;
      TIMA = TIMA + 1;
      if (TIMA == 0x00) {
        TIMA = TMA;
        // TODO THIS:
        // request_interrupt()
      }
    }
  }
}
