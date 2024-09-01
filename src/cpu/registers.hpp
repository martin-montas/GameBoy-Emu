#ifndef REGISTERS_HPP
#define REGISTERS_HPP

#include <cstdint>
class Registers {
    public:

        void HL(uint8_t &h, uint8_t &l);
        void AF(uint8_t &a, uint8_t &f);
        void BC(uint8_t &b, uint8_t &c);
        void DE(uint8_t &d, uint8_t &e);

    private:
        int8_t a; 
        int8_t b; 
        int8_t c; 
        int8_t d; 
        int8_t e; 
        int8_t h; 
        int8_t l;

        uint16_t hl;
        uint16_t af;
        uint16_t bc;
        uint16_t de;
};


#endif // REGISTERS_HPP
