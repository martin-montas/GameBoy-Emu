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

enum Interrupt_Flags {
    INTERRUPT_JOYPAD = (1 << 4),
    INTERRUPT_SERIAL = (1 << 3),
    INTERRUPT_TIMER  = (1 << 2),
    INTERRUPT_LCD    = (1 << 1),
    INTERRUPT_VBLANK = 1,
};

class Interrupt : public IInterrupt {

  public:
    uint8_t _IF;
    uint8_t _IE;
    void    request_interrupt(uint8_t interrupt_bit) override {
        _IF |= (1 << interrupt_bit);
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
        if ((IF & INTERRUPT_VBLANK) && (IE & INTERRUPT_VBLANK))
            return 0x0040;
        else if ((IF & INTERRUPT_LCD) && (IE & INTERRUPT_LCD))
            return 0x0048;
        else if ((IF & INTERRUPT_TIMER) && (IE & INTERRUPT_TIMER))
            return 0x0048;
        else if ((IF & INTERRUPT_SERIAL) && (IE & INTERRUPT_SERIAL))
            return 0x0050;
        else
            return 0x0060;
    }

  private:
};

#endif // SRC_INTERRUPT_HPP_
