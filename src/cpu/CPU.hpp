#ifndef CPU_HPP
#define CPU_HPP

#include <cstdint>
#include <vector>
#include <functional>
#include <array>
#include <string>

#include "registers.hpp" 

class CPU {

    public:
        CPU();
        ~CPU();

        void read_rom();
        std::vector<uint8_t> load_rom(const std::string &filename);
        void read_rom(const std::vector<uint8_t> &romData);

        // opcodes
        void init_opcode_table();

        void load_bc_nn();
        void nop();
        void nn();
        void load_bc_a();
        
    private:
        Registers *register;
        std::array<std::function<void()>, 256> opcode_table; 
        uint16_t pc;
        uint16_t sp;
        std::vector<uint8_t> romData;

        uint32_t cycle;
        uint32_t cycle_count;
};


#endif // CPU_HPP
