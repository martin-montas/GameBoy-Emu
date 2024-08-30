#ifndef REGISTERS_HPP
#define REGISTERS_HPP

class Registers {
    public:
        uint16_t pc = 0;
        uint16_t sp;

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
};


#endif // REGISTERS_HPP
