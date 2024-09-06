#ifndef MMU_HPP
#define MMU_HPP

#include <cstdint>

class MMU {
    public:
        int8_t HRAM[0x7F];
        int8_t IRAM[8192];

        int8_t VRAM[8192];

};

#endif // MMU_HPP
