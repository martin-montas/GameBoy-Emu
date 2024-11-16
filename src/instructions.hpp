// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
//
#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP


#include "./memory/MMU.hpp"
#include "./cpu/CPU.hpp"

class InstructionSet {
    private:
        MMU &mmu;
        CPU &cpu;
    public:
        InstructionSet(CPU &cpu, MMU &mmu);
        void execute(uint8_t opcode);

        void rrca(uint8_t reg);
        void rra();
        void rla();
        void ret(bool condition);

        void inc(uint8_t value);
        void inc(uint16_t value);
        void inc_mem(uint16_t reg);
        void inc_mem(uint8_t reg);

        void dec(uint8_t value);
        void dec(uint16_t value);
        void dec_mem(uint16_t value);
        void dec_mem(uint8_t value);

        void add8(uint8_t destination, uint8_t value);
        void add8_mem(uint8_t destination, uint8_t value);
        void add16(uint16_t destination, uint16_t value);

        void ldr(uint16_t reg);
        void ldr(uint8_t reg, uint8_t  address);
        void ldr_mem(uint16_t reg,uint8_t  address);

        void adc(uint8_t& reg_1, uint8_t reg_2);
        void sbc(uint8_t reg_1, uint8_t reg_2);
        void sub(uint8_t reg_1, uint8_t reg_2);
        void and_(uint8_t reg_1, uint8_t reg_2);
        void xor_(uint8_t reg_1, uint8_t reg_2);
        void or_(uint8_t reg_1, uint8_t reg_2); 

        void cp_(uint8_t reg_1, uint8_t reg_2);
        void call(bool condition);
        void jump(bool condition);
        void jump_add(bool condition);
        void cp_n(uint8_t value);
        void cpl(uint8_t reg);
        void rlc(uint8_t reg);

        void rlc_extended(uint8_t reg);
        void rcc_extended(uint8_t reg);

};
#endif
