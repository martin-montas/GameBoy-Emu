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
    INTERRUPT_JOY    = (1 << 4),
    INTERRUPT_SERIAL = (1 << 3),
    INTERRUPT_TIMER  = (1 << 2),
    INTERRUPT_LCD    = (1 << 1),
    INTERRUPT_VBLANK = 1,
};

class Interrupt : public IInterrupt {

  public:
    void request_interrupt(uint8_t interrupt_bit) override {
        _IF |= (1 << interrupt_bit);
    }

  private:
    uint8_t _IF;
};

#endif // SRC_INTERRUPT_HPP_
