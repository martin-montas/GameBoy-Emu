#ifndef SRC_SYSTEM_BUS_HPP_
#define SRC_SYSTEM_BUS_HPP_

#include <cstdint>

class SystemBus {

  public:
    virtual void    write8(uint16_t addr, uint8_t value) = 0;
    virtual uint8_t read8(uint16_t addr)                 = 0;

    virtual void     write16(uint16_t addr, uint16_t value) = 0;
    virtual uint16_t read16(uint16_t addr)                  = 0;

    virtual uint8_t getIE() = 0;
    virtual uint8_t getIF() = 0;

    virtual void setIF(uint8_t value) = 0;
};

#endif // SRC_SYSTEM_BUS_HPP_
