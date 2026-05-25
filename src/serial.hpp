// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
#ifndef SRC_SERIAL_HPP_
#define SRC_SERIAL_HPP_

#include <stdint.h>

#define TRANSFER_STATUS (1 << 7)
#define CLOCK_STATUS (1 << 1)

#define check_flag(value, bit) ((value) & (bit))

/*
 * @brief: this class deals with everything realated to
 * the GameBoy's SB and SC addresses (0xFF01,0xFF02
 respectively).
 *
 */
class Serial {

public:
  Serial()
      : _sb(0x00), _sc(0x7E), _extern_clock_mode(0), _acc(0), _shift_reg(0),
        _bit_count(0), t_cycle(0) {}

  void tick(int cycle);
  void write_sb(uint8_t value);
  void write_sc(uint8_t value);
  // uint8_t read_sb();
  // uint8_t read_sc();

private:
  uint8_t _sb;
  uint8_t _sc;
  bool _extern_clock_mode;
  bool _transfer_active;
  int t_cycle;
  int _acc;

  uint8_t _shift_reg;
  uint8_t _bit_count;

  // void start_transfer();
  void shift_one_bit();
};

#endif // SRC_SERIAL_HPP_
