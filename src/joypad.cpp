// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com

#include <iostream>
#include "joypad.hpp"

bool JoyPad::is_dpad_on() {
    bool dpad = !((_joy_buttons >> 4) & 1);
    if (dpad) {
        return true;
    }
    return false;
}

bool JoyPad::is_select_keys_on() {
    bool select = !((_joy_buttons >> 5) & 1);
    if (select) {
        return true;
    }
    return false;
}

void JoyPad::press_button(uint8_t bit) {
    _joy_buttons &= ~(1 << bit);
}

void JoyPad::release_button(uint8_t bit) {
    _joy_buttons |= (1 << bit);
}

uint8_t JoyPad::read() {
    return _joy_buttons;
}

uint8_t JoyPad::write(uint8_t val) {
    // _joy_buttons = val;
    _joyp_selection = (val >> 3) & 1;
}
