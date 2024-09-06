#ifndef MMU_HPP
#define MMU_HPP

#include <cstdint>

class MMU {
    public:
        uint8_t HRAM[0x7F];
        uint8_t IRAM[8192];

        uint8_t VRAM[0x2000];
        uint8_t WRAM[0x2000];
        uint8_t OAM[0xA0];

        uint8_t IO_REGISTERS[0x80];
        uint8_t EXTERNAL_RAM[8192];

        uint8_t InterruptEnabled;
        uint8_t read(uint16_t address);
        uint8_t write();
};

#endif // MMU_HPP
