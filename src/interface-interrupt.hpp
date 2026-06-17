#ifndef INTERFACE_IINTERRUPT_HPP
#define INTERFACE_IINTERRUPT_HPP

#include <stdint.h>
#include "cpu.hpp"
/*
 * @brief: helper for interrupt bitwise
 * operations. used for both: the interrupt
 * flag and the interrupt enable registers
 */
enum Interrupt_Flags {
    INTERRUPT_JOYPAD = (1 << 4),
    INTERRUPT_SERIAL = (1 << 3),
    INTERRUPT_TIMER  = (1 << 2),
    INTERRUPT_LCD    = (1 << 1),
    INTERRUPT_VBLANK = 1,
};

/*
 * @brief: interrupt interface, where most of its members
 * get inmplemented by the actual interrupt class.
 */
class Cpu;
class IInterrupt {
  public:
    uint8_t _IF; /* interruopt flag */
    uint8_t _IE; /*  interrupt eanble */

    virtual ~IInterrupt()                                         = default;
    virtual void     request_exec_interrupt(Interrupt_Flags flag) = 0;
    virtual bool     pending_interrupt()                          = 0;
    virtual uint16_t get_interrupt_vector()                       = 0;
    virtual uint8_t  read(uint16_t addr)                          = 0;
    virtual void     write(uint16_t addr, uint8_t value)          = 0;

  private:
    Interrupt_Flags _interrupt;
};

#endif // INTERFACE_IINTERRUPT_HPP
