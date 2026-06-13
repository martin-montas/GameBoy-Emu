// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
//
#ifndef SRC_INTERRUPT_HPP_
#define SRC_INTERRUPT_HPP_

#include <stdint.h>
#include "interface-interrupt.hpp"

#define IE 0xFFFF
#define IF 0xFF0F

/*
 * @brief: implementation of the IInterrupt class.
 * deals with handling of states and execution of
 * the interrupt subsystem of the GameBoy
 *
 */
class Interrupt : public IInterrupt {
  public:
    uint8_t _IF; /* pointer to interrupt flag reg */
    uint8_t _IE; /* pointer to interrupt enable reg */

    void request_interrupt(Interrupt_Flags flag) override {
        _IF |= flag;
    }

    uint8_t read(uint16_t addr) override {
        if (addr == 0xFFFF) {
            return _IE;
        }
        return _IF;
    }
    void write(uint16_t addr, uint8_t value) override {
        if (addr == 0xFFFF)
            _IE = value;
        else if (addr == 0xFF0F)
            _IF = value;
    }

    bool pending_interrupt() override {
        if (((_IF & _IE) & 0x1F) != 0) {
            return true;
        }
        return false;
    }

    uint8_t get_interrupt_vector() override {
        if ((_IF & INTERRUPT_VBLANK) && (_IE & INTERRUPT_VBLANK)) {
            return 0x0040;
        } else if ((_IF & INTERRUPT_LCD) && (_IE & INTERRUPT_LCD)) {
            return 0x0048;
        } else if ((_IF & INTERRUPT_TIMER) && (_IE & INTERRUPT_TIMER)) {
            return 0x0048;
        } else if ((_IF & INTERRUPT_SERIAL) && (_IE & INTERRUPT_SERIAL)) {
            return 0x0050;
        } else {
            return 0x0060;
        }
    }
};

#endif // SRC_INTERRUPT_HPP_
