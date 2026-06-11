// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
//
#ifndef SRC_INTERRUPT_HPP_
#define SRC_INTERRUPT_HPP_

#include <stdint.h>

#define IE 0xFFFF
#define IF 0xFF0F

enum Interrupt_Flags {
    FLAG_JOY    = (1 << 4),
    FLAG_SERIAL = (1 << 3),
    FLAG_TIMER  = (1 << 2),
    FLAG_LCD    = (1 << 1),
    FLAG_VBLANK = 1,
};

class Interrupt {
  private:
  public:
    void request_interrupt(uint16_t addr);
    void enable();
};

#endif // SRC_INTERRUPT_HPP_
