#ifndef SRC_DEVICE_HPP_
#define SRC_DEVICE_HPP_

#include <stdint.h>
#include <cstddef>
#define VBLANK_IRQ 0x01
#define PPU_IRQ    0x02
#define TIMER_IRQ  0x04
#define SERIAL_IRQ 0x08
#define JOYPAD_IRQ 0x10

class Device {
  protected:
    uint8_t* _IF; /* interrupt flag register */

  public:
    Device() : _IF(NULL) {};

    virtual void post_irq(uint8_t value) {
        if (_IF) {
            *_IF |= value;
        }
    }

    virtual void set_irq(uint8_t* _IF) {
        _IF = _IF;
    }

    virtual void cycle() {}

    virtual uint8_t read(uint16_t addr)                 = 0;
    virtual void    write(uint16_t addr, uint8_t value) = 0;
};

#endif // SRC_DEVICE_HPP_
