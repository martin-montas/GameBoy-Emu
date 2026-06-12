#ifndef _SRC_IINTERRUPT_HPP_
#define _SRC_IINTERRUPT_HPP_

#include <stdint.h>

class IInterrupt {
  public:
    virtual ~IInterrupt()                                    = default;
    virtual void    request_interrupt(uint8_t interrupt_bit) = 0;
    virtual uint8_t read(uint16_t addr)                      = 0;
    virtual void    write(uint16_t addr, uint8_t value)      = 0;
};

#endif // _SRC_IINTERRUPT_HPP_
