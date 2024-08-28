#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cstdint>

class Memory {
    public:

    private:

        // The high ram of a game boy is 127 bytes
        const int8_t HRAM[0x7F];

        // Internal ram of a game boy is 8 KB 
        const int8_t IRAM[8192];

        // Video ram of a game boy is 8 KB 
        const int8_t VRAM[8192];
};

#endif // MEMORY_HPP
