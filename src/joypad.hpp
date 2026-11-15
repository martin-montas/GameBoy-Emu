// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#ifndef JOYPAD_HPP_
#define JOYPAD_HPP_

#include <cstdint>
#include <stdint.h>
#include "interface-interrupt.hpp"

// $FF00
// Bit 5 = P15 = select button group
// Bit 4 = P14 = select direction group
//
// Bit 3 = Down / Start
// Bit 2 = Up   / Select
// Bit 1 = Left / B
// Bit 0 = Right / A

enum JoyReg {
    SELECT_BUTTONS    = (1 << 5), /* for parsing the button bit */
    SELECT_D_PAD      = (1 << 4), /* for parsing the directions */
    BUTTON_START_DOWN = (1 << 3), /* selects Down/Start Button */
    BUTTON_SELECT_UP  = (1 << 2), /* selects Up/Select Button */
    BUTTON_B_LEFT     = (1 << 1), /* selects Left/B Button */
    BUTTON_A_RIGHT    = (1 << 0), /* selects Right/A Button */
};

class IInterrupt;
class JoyPad {
    uint8_t     _joy_buttons;    /* main joypad register */
    IInterrupt* _interrupt;      /* pointer to interrupt object */
    uint8_t     _joyp_selection; /* distinguish between direction/other key type */
    uint8_t     _buttons;        /* uses the 4 first bits for the keys type */

  public:
    explicit JoyPad() : _joy_buttons(0) {};
    void    release_button(uint8_t bit);
    void    press_button(uint8_t bit);
    uint8_t read();
    uint8_t write(uint8_t value);
    void    set_button();
    void    check_register();
    bool    is_dpad_on();
    bool    is_select_keys_on();
};

#endif // !JOYPAD_HPP_
