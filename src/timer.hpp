// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
//
#ifndef SRC_TIMER_HPP_
#define SRC_TIMER_HPP_

#include <stdint.h>
#include "./interface-interrupt.hpp"
#include "device.hpp"

#define TAC_SELECT 0b11

enum TAC_TIMER_CONTROL : uint8_t { TIMER_0 = 0b00, TIMER_1 = 0b01, TIMER_2 = 0b10, TIMER_3 = 0b11 };

class Timer : public Device {
    uint8_t     _tma;           /* Timer reload controller */
    uint8_t     _tima;          /* Current tima register */
    uint16_t    _div;           /* Div register */
    int         _div_acc;       /* Used for syncing */
    int         _tima_acc;      /* Used for syncing */
    IInterrupt* _interrupt;     /* pointer to interrupt object */
    int         curr_frequency; /* used for getting frequency t cycles */
    uint8_t     _tac;           /* Timer control register */

  public:
    explicit Timer(IInterrupt* interrupt)
        : _div(0), _tima(0), _tma(0), _tac(0), _interrupt(interrupt), _div_acc(0), _tima_acc(0) {}

    /*
     * @brief: this happens every iteration of the game loop
     * @param[in]: current m-cycle for the instruction
     */
    void tick(int cycles);

    /*
     * @brief: the tag flag should be set if value paramenter has the
     * bit 0 flipped
     */
    // uint8_t get_tac_flag();

    void set_tac_flag(TAC_TIMER_CONTROL tac_timer);

    /*
     * @brief: When the the Gameboy changes speeds
     * it alters the Gigahertz and it time per instructions
     * alters as well.
     * @returns: time per instruction number.
     */
    int clock_speed();

    /*
     * @brief: This method writes to the register.
     * Use for the timin of the gameboy.
     * @param(in): Addresss to write to.
     * @param_2(in): value to write to any of the timing registers.
     */
    void write(uint16_t addr, uint8_t value);

    /*
     * @brief: Reads the timing registers.
     * @param(in): Address of the giving timing register.
     * @return: Retuns the given timing register.
     */
    uint8_t read(uint16_t addr);
    uint8_t write(uint16_t addr);
};

#endif // !SRC_TIMER_HPP_
