#ifndef CPU_HPP
#define CPU_HPP

#include <cstdint>
#include <vector>

struct Registers {
    int8_t a; 
    int8_t b; 
    int8_t c; 
    int8_t d; 
    int8_t e; 
    int8_t h; 
    int8_t l;
};

struct FlagsRegister {
    bool zero;
    bool subtract;
    bool halfCarry;
    bool carry;
}


class Cpu {

    public:
        Cpu();
        ~Cpu();

        void readOpcodes();

        std::vector<uint8_t> loadRom(const std::string &filename);
    private:
        Registers registers;

        std::vector<uint8_t> rom_data(size);

        uint16_t pc;
        uint8_t opcode;
 
};

#endif // CPU_HPP
