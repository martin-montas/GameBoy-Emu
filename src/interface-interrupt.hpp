#ifndef _SRC_IINTERRUPT_HPP_
#define _SRC_IINTERRUPT_HPP_

#include <stdint.h>

enum Interrupt_Flags {
    INTERRUPT_JOYPAD = (1 << 4),
    INTERRUPT_SERIAL = (1 << 3),
    INTERRUPT_TIMER  = (1 << 2),
    INTERRUPT_LCD    = (1 << 1),
    INTERRUPT_VBLANK = 1,
};
class IInterrupt {
  public:
    virtual ~IInterrupt()                                         = default;
    virtual void    request_interrupt(Interrupt_Flags _interrupt) = 0;
    virtual bool    pending_interrupt()                           = 0;
    virtual uint8_t get_interrupt_vector()                        = 0;
    virtual uint8_t read(uint16_t addr)                           = 0;
    virtual void    write(uint16_t addr, uint8_t value)           = 0;

  private:
    Interrupt_Flags _interrupt;
};

#endif // _SRC_IINTERRUPT_HPP_
