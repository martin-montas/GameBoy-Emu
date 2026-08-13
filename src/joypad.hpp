// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#ifndef JOYPAD_HPP_
#define JOYPAD_HPP_

#include <stdint.h>
#include "bus.hpp"

// $FF00
// Bit 5 = P15 = select button group
// Bit 4 = P14 = select direction group
//
// Bit 3 = Down / Start
// Bit 2 = Up   / Select
// Bit 1 = Left / B
// Bit 0 = Right / A

enum JoyReg {
    SELECT_BUTTONS    = (5 << 1), /* for parsing the button bit */
    BUTTON_D_PAD      = (4 << 1), /* for parsing the directions */
    BUTTON_START_DOWN = (3 << 1), /* selects Down/Start Button */
    BUTTON_SELECT_UP  = (2 << 1), /* selects Up/Select Button */
    BUTTON_B_LEFT     = (1 << 1), /* selects Left/B Button */
    BUTTON_A_RIGHT    = (0 << 1), /* selects Right/A Button */

};

class JoyPad {

    Bus* _bus; /* pointer to the bus instance */

  public:
    explicit JoyPad();
    void write(uint8_t reg, uint8_t val);
    void read(uint8_t reg);

    void set_button();
};

#endif // !JOYPAD_HPP_
