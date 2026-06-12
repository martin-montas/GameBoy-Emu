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

class Interrupt : public IInterrupt {

  public:
    uint8_t _IF;
    uint8_t _IE;

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

    uint8_t get_interrupt_vector() {
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

  private:
};

#endif // SRC_INTERRUPT_HPP_
