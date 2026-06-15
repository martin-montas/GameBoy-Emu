// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#ifndef SRC_INSTRUCTIONS_HPP_
#define SRC_INSTRUCTIONS_HPP_

// #include "system-bus.hpp"
#include "cpu.hpp"
#include <cstdint>
#include <stdint.h>

class SystemBus;
class Cpu;
/*
 * @brief: Holds the implementation of the
 * instructions of the cpu. It holds them
 * in 2 files.
 */
class InstructionSet {
    SystemBus* _mmu; /* object to mem class */
    Cpu*       _cpu; /* object to cpu class */

  public:
    InstructionSet(SystemBus* mmu, Cpu* cpu) : _mmu(mmu), _cpu(cpu) {
        /* initializes regs */
        pre_boot_state();
    }

    void step();
    void execute(uint8_t opcode);
    void pre_boot_state();
    void post_boot_state();

    void get_mbc_type();
    void rrca(uint8_t& reg);
    void rra();
    void rla();
    void pop_(bool condition, uint16_t& reg);
    void ret(bool condition);

    void generate_mbc();
    void generate_mbcinc(uint8_t& reg);

    void inc(uint16_t& reg);
    void inc(uint8_t& reg);
    void inc_mem(uint16_t& reg);

    void dec(uint8_t& reg);
    void dec_mem(uint16_t& value);

    void add8(uint8_t& reg_1, uint8_t reg_2);
    void add16(uint16_t& destination, uint16_t& value);
    void add8_mem(uint8_t& destination, uint8_t value);

    void ldr(uint16_t& reg);
    void ldr(uint8_t& reg);
    void ld_mem(uint8_t& reg, uint16_t addr); // for loading to 16 addresses
    void ld(uint8_t& reg, uint8_t addr);      // for loading 8 bit addresses
    void ld(uint16_t& reg, uint16_t addr);

    void adc(uint8_t& reg_1, uint8_t reg_2);
    void sbc(uint8_t& reg_1, uint8_t reg_2);
    void and_(uint8_t& reg_1, uint8_t reg_2);
    void xor_(uint8_t& reg_1, uint8_t reg_2);
    void sub(uint8_t& reg_1, uint8_t reg_2);
    void or_(uint8_t& reg_1, uint8_t reg_2);
    void cp_(uint8_t reg_1, uint8_t reg_2);
    void call(bool condition);

    void execute_call();
    void jump(bool condition, int8_t offset);
    void jump_add(bool condition);
    void cp_n(uint8_t value);
    void push_(uint16_t reg);
    void cpl(uint8_t& reg);
    void rlc(uint8_t reg);

    void rlc_extended(uint8_t& reg);
    void sla_extended(uint8_t& reg);
    void rcc_extended(uint8_t* reg);
    void rrc_extended(uint8_t& reg);
    void rl_extended(uint8_t& reg);
    void srl_extended(uint8_t& reg);

    void rr_extended(uint8_t& reg);
    void sra_extended(uint8_t& reg);

    void swap_extended(uint8_t& reg);

    void bit0_extended(uint8_t& reg);
    void bit1_extended(uint8_t& reg);
    void bit2_extended(uint8_t& reg);
    void bit3_extended(uint8_t& reg);
    void bit4_extended(uint8_t& reg);
    void bit5_extended(uint8_t& reg);
    void bit6_extended(uint8_t& reg);
    void bit7_extended(uint8_t& reg);

    void res0_extended(uint8_t& reg);
    void res1_extended(uint8_t& reg);
    void res2_extended(uint8_t& reg);
    void res3_extended(uint8_t& reg);
    void res4_extended(uint8_t& reg);
    void res5_extended(uint8_t& reg);
    void res6_extended(uint8_t& reg);
    void res7_extended(uint8_t& reg);

    void set0_extended(uint8_t& reg);
    void set1_extended(uint8_t& reg);
    void set2_extended(uint8_t& reg);
    void set3_extended(uint8_t& reg);
    void set4_extended(uint8_t& reg);
    void set5_extended(uint8_t& reg);
    void set6_extended(uint8_t& reg);
    void set7_extended(uint8_t& reg);

    void interrupt_handler();
};
#endif // SRC_INSTRUCTIONS_HPP_
