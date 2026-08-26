// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com

#include <iostream>
#include "joypad.hpp"

void JoyPad::write(uint8_t val) {
    _JOYP = val;
}
uint8_t JoyPad::read(uint8_t reg) {
    return _JOYP;
}
void JoyPad::set_button() {
    if (!(_JOYP & SELECT_BUTTONS)) {
        if (!(_JOYP & BUTTON_START_DOWN)) {
        }
        if (!(_JOYP & BUTTON_SELECT_UP)) {
        }
        if (!(_JOYP & BUTTON_B_LEFT)) {
        }
        if (!(_JOYP & BUTTON_A_RIGHT)) {
        }
    }
    if (!(_JOYP & SELECT_D_PAD)) {
        if (!(_JOYP & BUTTON_START_DOWN)) {
        }
        if (!(_JOYP & BUTTON_SELECT_UP)) {
        }
        if (!(_JOYP & BUTTON_B_LEFT)) {
        }
        if (!(_JOYP & BUTTON_A_RIGHT)) {
        }
    }
}
