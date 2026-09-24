// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#ifndef SRC_SERIAL_HPP_
#define SRC_SERIAL_HPP_

#include <stdint.h>
#include "device.hpp"

#define TRANSFER_STATUS (1 << 7)
#define CLOCK_SPEED     (1 << 1)
#define CLOCK_STATUS    1

/*
 * @brief: this class deals with everything related to
 * the GameBoy's SB and SC addresses (0xFF01,0xFF02
 respectively).
 */
class Serial : public Device {
    uint8_t _sb;                /* serial transfer data */
    uint8_t _sc;                /* serial transfer control */
    bool    _extern_clock_mode; /* updates clock mode */
    int     _acc;
    uint8_t _shift_reg;
    uint8_t _bit_count;

    void shift_one_bit();

  public:
    Serial() : _sb(0x00), _sc(0x7E), _extern_clock_mode(0), _acc(0), _shift_reg(0), _bit_count(0) {}

    /*
     * @brief:  this function runs every instruction
     * execution and keeps accumulating the t_cycles.
     *
     * @param[in]: t_cycle per instructions.
     */
    void    tick(int cycle);
    void    write(uint16_t addr, uint8_t value) override;
    uint8_t read(uint16_t addr) override;
};

#endif // SRC_SERIAL_HPP_
