#ifndef CPU_HPP
#define CPU_HPP

#include <cstdint>
#include <vector>
#include <string>
#include <array>
#include <functional>

#include "../memory/MMU.hpp" 

enum RegisterFlags { 
    FLAG_ZERO = (1 << 7), FLAG_SUBTRACT = (1 << 6), FLAG_HALF_CARRY = (1 << 5), FLAG_CARRY = (1 << 4)
};

class CPU {
    private:
        MMU &mmu;

    public:
        CPU(MMU &mmu);
        ~CPU();

        bool is_flag_set(uint8_t flag);
        void set_flags(uint8_t flags, bool state);
        void print_flags();

        void step();
        std::vector<uint8_t> load_rom(const std::string &filename);
        uint32_t execute_opcode(uint8_t opcode);
        void emulate_cycles(uint32_t cyclesToRun);

        void init_opcode_table();

        const int opcode_cycles[256] = {
            4, 12, 8, 8, 4, 4, 8, 4, 20, 8, 8, 8, 4, 4, 8, 4,       // 0x0_
            4, 12, 8, 8, 4, 4, 8, 4,  12, 8, 8, 8, 4, 4, 8, 4,      // 0x1_
            0, 12, 8, 8, 4, 4, 8, 4,  0, 8, 8, 8, 4, 4, 8, 4,       // 0x2_
            0, 12, 8, 8, 12, 12, 12, 4,  0, 8, 8, 8, 4, 4, 8, 4,    // 0x3_
            4, 4, 4, 4, 4, 4, 8, 4,  4, 4, 4, 4, 4, 4, 8, 4,        // 0x4_
            4, 4, 4, 4, 4, 4, 8, 4,  4, 4, 4, 4, 4, 4, 8, 4,        // 0x5_
            4, 4, 4, 4, 4, 4, 8, 4,  4, 4, 4, 4, 4, 4, 8, 4,        // 0x6_
            8, 8, 8, 8, 8, 8, 4, 8,  4, 4, 4, 4, 4, 4, 8, 4,        // 0x7_
            4, 4, 4, 4, 4, 4, 8, 4,  4, 4, 4, 4, 4, 4, 8, 4,        // 0x8_
            4, 4, 4, 4, 4, 4, 8, 4,  4, 4, 4, 4, 4, 4, 8, 4,        // 0x9_
            4, 4, 4, 4, 4, 4, 8, 4,  4, 4, 4, 4, 4, 4, 8, 4,        // 0xa_
            4, 4, 4, 4, 4, 4, 8, 4,  4, 4, 4, 4, 4, 4, 8, 4,        // 0xb_
            0, 12, 0, 16, 0, 16, 8, 16,  0, 16, 0, 0, 0, 24, 8, 16, // 0xc_
            0, 12, 0, 0, 0, 16, 8, 16,  0, 16, 0, 0, 0, 0, 8, 16,   // 0xd_
            12, 12, 8, 0, 0, 16, 8, 16,  16, 4, 16, 0, 0, 0, 8, 16, // 0xe_
            12, 12, 8, 4, 0, 16, 8, 16,  12, 8, 16, 4, 0, 0, 8, 16  // 0xf_
        };
        std::array<std::function<void()>, 256> opcode_table; 
        struct {
            union {
                struct {
                    uint8_t F;
                    uint8_t A;
                };
                uint16_t AF;
            };
        };

        struct {
            union {
                struct {
                    uint8_t C;
                    uint8_t B;
                };
                uint16_t BC;
            };
        };  

        struct {
            union {
                struct {
                    uint8_t E;
                    uint8_t D;
                };
                uint16_t DE;
            };
        };

        struct {
            union {
                struct {
                    uint8_t L;
                    uint8_t H;
                };
                uint16_t HL;
            };
        };
        uint16_t SP;
        uint16_t PC;
        uint32_t cycle_count;

    private:

        uint32_t cycle;
        uint32_t globalCycles;

        // program counter
        uint16_t pc;

        // stack pointer
        uint16_t sp;

};

#endif // CPU_HPP
