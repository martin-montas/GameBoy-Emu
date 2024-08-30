#ifndef MMU_HPP
#define MMU_HPP

#include <cstdint>

class MMU {
    public:
    private:

        const int8_t HRAM[0x7F];
        const int8_t IRAM[8192];

        const int8_t VRAM[8192];
};

#endif // MMU_HPP
