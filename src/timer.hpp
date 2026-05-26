// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
//
#ifndef SRC_TIMER_HPP_
#define SRC_TIMER_HPP_

#include <stdint.h>

#define TAC_SELECT 0b11

enum TAC_TIMER_CONTROL : uint8_t { TIMER_0 = 0b00, TIMER_1 = 0b01, TIMER_2 = 0b10, TIMER_3 = 0b11 };

class Timer {
private:
  uint8_t  _sb;
  uint8_t  _sc;
  uint8_t  _tima;
  uint8_t  _tma;
  uint8_t  _tac;
  uint32_t _div;
  uint8_t  _tima_accumulator;
  uint16_t _div_counter;

  int      curr_frequency = 256;

public:
  Timer()
      : _sb(0), _div(0), _sc(0), _tima(0), _tma(0), _tac(0), _tima_accumulator(0), _div_counter(0) {
  }

  /*
   * @brief: this happens every iteration of the game loop
   * @param[in]: current m-cycle for the instruction
   */
  void    tick(int cycle);

  // /*
  //  * @brief: the tag flag should be set if value paramenter has the
  //  * bit 0 flipped
  //  */
  // uin8_t get_tac_flag();

  void    set_tac_flag(TAC_TIMER_CONTROL tac_timer);

  /*
   * @brief: When the the Gameboy changes speeds
   * it alters the Gigahertz and it time per instructions
   * alters as well.
   * @returns: time per instruction number.
   */
  int     get_time_frequency();

  /*
   * @brief: This method writes to the register.
   * Use for the timing of the gameboy.
   * @param(in): Addresss to write to.
   * @param_2(in): value to write to any of the timing registers.
   */
  void    write(uint16_t addr, uint8_t value);

  /*
   * @brief: Reads the timing registers.
   * @param(in): Address of the giving timing register.
   * @return: Retuns the given timing register.
   */
  uint8_t read(uint16_t addr);

  uint8_t write(uint16_t addr);
};
#endif // !SRC_TIMER_HPP_
