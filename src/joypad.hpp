// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#ifndef JOYPAD_HPP_
#define JOYPAD_HPP_
#include <stdint.h>

enum JoyReg {
    SELECT_BUTTONS    = (5 << 1),
    BUTTON_D_PAD      = (4 << 1),
    BUTTON_START_DOWN = (3 << 1),
    BUTTON_SELECT_UP  = (2 << 1),
    BUTTON_B_LEFT     = (1 << 1),
    BUTTON_A_LEFT     = (0 << 1),

};

class JoyPad {
  public:
    void write(uint8_t reg, uint8_t val);
    void read(uint8_t reg);
};

#endif // !JOYPAD_HPP_
