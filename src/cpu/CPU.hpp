#ifndef CPU_HPP
#define CPU_HPP

#include <cstdint>
#include <vector>

#include "registers.hpp" 

class Cpu {

    public:
        Cpu();
        ~Cpu();

        void read_rom();
        std::vector<uint8_t> load_rom(const std::string &filename);

    private:
        Registers *register;
        std::vector<uint8_t> rom_data(size);

        uint16_t pc;
        uint16_t sp;

};

#endif // CPU_HPP
