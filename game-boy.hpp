#ifndef GAME_BOY_HPP
#define GAME_BOY_HPP

#include <cstdint>

struct Registers {
    int8_t a; 
    int8_t b; 
    int8_t c; 
    int8_t d; 
    int8_t e; 
    int8_t h; 
    int8_t l;
};
class GameBoy {

    public:
        GameBoy();
        ~GameBoy();

        void init();
        void run();
        void deinit();

    private:

        // Registers are defined here
        Registers registers;

        // The high ram of a game boy is 127 bytes
        const int8_t HRAM[0x7F];

        // Internal ram of a game boy is 8 KB 
        const int8_t IRAM[8192];

        // Video ram of a game boy is 8 KB 
        const int8_t VRAM[8192];
};

#endif // GameBoy
