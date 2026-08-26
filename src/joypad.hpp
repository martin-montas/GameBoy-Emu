// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#ifndef JOYPAD_HPP_
#define JOYPAD_HPP_

#include <stdint.h>
#include "bus.hpp"
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
    uint8_t     _JOYP;      /* main joypad register */
    Bus*        _bus;       /* pointer to the bus instance */
    IInterrupt* _interrupt; /* pointer to interrupt object */

  public:
    explicit JoyPad() {};
    void write(uint8_t reg, uint8_t val);
    void read(uint8_t reg);
    void set_button();
};

#endif // !JOYPAD_HPP_
