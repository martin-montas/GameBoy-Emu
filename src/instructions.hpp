// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
#ifndef SRC_INSTRUCTIONS_HPP_
#define SRC_INSTRUCTIONS_HPP_

#include <cstdint>

#include "mmu.hpp"
#include "cpu.hpp"

class InstructionSet {
 private:
     static Cpu *cpu;
     static MMU *mmu;
 public:
     // Getter to access the Cpu object for testing
    Cpu* getCpu() const {
        return cpu;
    }
    ~InstructionSet() { delete cpu; }
     InstructionSet(MMU* mmu, Cpu *cpu);
     void execute(uint8_t opcode);

     static void rrca(uint8_t reg);
     static void rra();
     static void rla();
     static void ret(bool condition);

     static void inc(uint8_t value);
     static void inc(uint16_t value);
     static void inc_mem(uint16_t reg);
     static void inc_mem(uint8_t reg);

     static void dec(uint8_t value);
     static void dec(uint16_t value);
     static void dec_mem(uint16_t value);
     static void dec_mem(uint8_t value);

     static void add8(uint8_t destination, uint8_t value);
     static void add8_mem(uint8_t destination, uint8_t value);
     static void add16(uint16_t destination, uint16_t value);

     static void ldr(uint16_t reg);
     static void ldr(uint8_t reg, uint8_t  address);
     static void ldr_mem(uint16_t reg, uint8_t address);

      // static void adc(uint8_t reg_1, uint8_t reg_2);
      // static void sbc(uint8_t *reg_1, uint8_t *reg_2);
      // static void and_(uint8_t *reg_1, uint8_t *reg_2);
      // static void xor_(uint8_t *reg_1, uint8_t *reg_2);
     static void sub(uint8_t  *reg_1,  uint8_t  *reg_2);
     static void or_(uint8_t  *reg_1,  uint8_t  *reg_2);
     static void cp_(uint8_t  * reg_1, uint8_t  *reg_2);
     static void call(bool condition);
     static void jump(bool condition);
     static void jump_add(bool condition);
     static void cp_n(uint8_t value);
     static void cpl(uint8_t reg);
     static void rlc(uint8_t reg);

     static void rlc_extended(uint8_t  *reg);
     static void rlc_extended_mem(uint16_t  *reg); // todo(soon)
     static void sla_extended(uint8_t  *reg);
     static void rcc_extended(uint8_t  *reg);
     static void rr_extended(uint8_t   *reg);
     static void rl_extended(uint8_t   *reg);
     static void sra_extended(uint8_t  *reg);
     static void swap_extended(uint8_t *reg);
     static void srl_extended(uint8_t  *reg);
};
#endif  // SRC_INSTRUCTIONS_HPP_
