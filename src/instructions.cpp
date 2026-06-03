// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
#include "instructions.hpp"
#include "MBC.hpp"
#include "MBC0.hpp"
#include "cpu.hpp"

#include <bit>
#include <cstdint>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

void InstructionSet::post_boot_state() {
    // 1. CPU Registers
    cpu->PC = 0x0100;
    cpu->SP = 0xFFFE;
    cpu->AF = 0x01B0;
    cpu->BC = 0x0013;
    cpu->DE = 0x00D8;
    cpu->HL = 0x014D;

    mmu->write8(0xFF01, 0x00); // SB (Serial Link Data Buffer)
    mmu->write8(0xFF02, 0x7E); // SC (Serial Link Control - Unused bits read as 1)

    // 2. Timers & System Interrupts
    mmu->write8(0xFF04, 0xAB); // DIV (Divider Register)
    mmu->write8(0xFF05, 0x00); // TIMA
    mmu->write8(0xFF06, 0x00); // TMA
    mmu->write8(0xFF07, 0x00); // TAC
    mmu->write8(0xFF0F, 0xE1); // IF (Interrupt Flag - Top 3 bits always 1)

    // 3. Audio Registers (APU)
    mmu->write8(0xFF10, 0x80);
    mmu->write8(0xFF11, 0xBF);
    mmu->write8(0xFF12, 0xF3);
    mmu->write8(0xFF14, 0xBF);
    mmu->write8(0xFF16, 0x3F);
    mmu->write8(0xFF17, 0x00);
    mmu->write8(0xFF19, 0xBF);
    mmu->write8(0xFF1A, 0x7F);
    mmu->write8(0xFF1B, 0xFF);
    mmu->write8(0xFF1C, 0x9F);
    mmu->write8(0xFF1E, 0xBF);
    mmu->write8(0xFF20, 0xFF);
    mmu->write8(0xFF21, 0x00);
    mmu->write8(0xFF22, 0x00);
    mmu->write8(0xFF23, 0xBF); // Fixed duplicate typo here
    mmu->write8(0xFF24, 0x77);
    mmu->write8(0xFF25, 0xF3);
    mmu->write8(0xFF26, 0xF1);

    // 4. Graphics Registers (PPU) - CRITICAL ADDITIONS
    mmu->write8(0xFF40, 0x91); // LCDC (Turns LCD Main Display ON)
    mmu->write8(0xFF42, 0x00); // SCY (Scroll Y)
    mmu->write8(0xFF43, 0x00); // SCX (Scroll X)
    mmu->write8(0xFF45, 0x00); // LYC
    mmu->write8(0xFF47, 0xFC); // BGP (Background Palette mapping)
    mmu->write8(0xFF48, 0xFF); // OBP0 (Object Palette 0)
    mmu->write8(0xFF49, 0xFF); // OBP1 (Object Palette 1)
    mmu->write8(0xFF4A, 0x00); // WY (Window Y)
    mmu->write8(0xFF4B, 0x00); // WX (Window X)

    // 5. Unmap Boot ROM
    mmu->write8(0xFF50, 0x01); // Unmaps Boot ROM, enabling Cartridge mapping

    // 6. Interrupt Enable
    mmu->write8(0xFFFF, 0x00); // IE (Disable all interrupts at boot)
}

InstructionSet::InstructionSet(MMU* mmu, Cpu* cpu) {
    this->cpu = cpu;
    this->mmu = mmu;
}

void InstructionSet::ldr(uint16_t& reg) {
    // DONE
    uint8_t  tmp_1 = this->mmu->romData[cpu->PC + 1];
    uint8_t  tmp_2 = this->mmu->romData[cpu->PC + 2];
    uint16_t tmp   = (tmp_2 << 8) | tmp_1;
    reg            = tmp;
}

void InstructionSet::ldr(uint8_t& reg) {
    uint8_t tmp = this->mmu->romData[this->cpu->PC + 1];
    reg         = tmp;
}

// DONE:
void InstructionSet::ld_mem(uint8_t& reg, uint16_t addr) {
    mmu->write8(addr, reg);
}

void InstructionSet::jump(bool condition, int8_t offset) {
    // DONE:
    cpu->PC += 2;
    if (condition) {
        cpu->PC += offset;
    }
}
void InstructionSet::inc(uint16_t& reg) {
    uint16_t old = reg;

    reg = old + 1;

    cpu->clear_flag(FLAG_SUBTRACT);
    cpu->set_flag(FLAG_HALF_CARRY, ((old & 0x0FFF) + 1) > 0x0FFF);
}
void InstructionSet::ld(uint16_t& reg, uint16_t addr) {
    reg = addr;
}
void InstructionSet::ld(uint8_t& reg, uint8_t addr) {
    reg = addr;
}

void InstructionSet::execute(uint8_t opcode) {
    switch (opcode) {
    case 0x00: {
        cpu->PC += 1;
        // printf("NOP  -- %X --\n", cpu->PC);
        break;
    }
    case 0x01: {
        uint8_t  l   = mmu->read8(cpu->PC + 1);
        uint8_t  h   = mmu->read8(cpu->PC + 2);
        uint16_t val = (h << 8) | l;
        printf("LD (BC), A 0x02 -- %X --\n", val);
        cpu->BC = val;
        cpu->PC = cpu->PC + 3;
        break;
    }
    case 0x02: {
        ld_mem(cpu->A, cpu->BC);
        printf("LD (BC), A 0x02 -- %X --\n", cpu->BC);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x03: {
        printf("INC BC 0x03 -- before %X --\n", cpu->BC);
        cpu->BC = cpu->BC + 1;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x04: {
        printf("INC B 0x04 -- before %X --\n", cpu->B);
        inc(cpu->B);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x05: {
        printf("DEC B 0x05 -- before %X --\n", cpu->B);
        dec(cpu->B);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x06: {
        printf("LD B, d8 0x06 --- before %X --\n", cpu->B);
        cpu->B  = mmu->romData[cpu->PC + 1];
        cpu->PC = cpu->PC + 2;
        break;
    }
    case 0x07: {
        uint16_t result = cpu->A << 1;
        bool     b      = (cpu->A << 8) & 1;
        printf("RLCA -- %d --\n", b);
        if (b) {
            cpu->A |= 1;
            cpu->set_flag(FLAG_CARRY, 1);
        } else {
            cpu->A &= ~1;
            cpu->set_flag(FLAG_CARRY, 0);
        }
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x08: {
        uint8_t  l       = mmu->romData[cpu->PC + 1];
        uint8_t  h       = mmu->romData[cpu->PC + 2];
        uint16_t address = (h << 8) | l;
        printf("LD (a16), SP -- %X --\n", address);
        uint8_t value = cpu->SP & 0xFF;
        mmu->write8(address, value);
        cpu->PC = cpu->PC + 3;
        break;
    }
    case 0x09: {
        cpu->HL      = cpu->HL + cpu->BC;
        uint32_t tmp = cpu->HL + cpu->BC;
        printf("ADD HL, BC -- %X --\n", cpu->HL);
        cpu->set_flag(FLAG_ZERO, 0);
        cpu->set_flag(FLAG_CARRY, tmp > 0xFFFF);
        cpu->set_flag(FLAG_SUBTRACT, ((cpu->HL & 0x0FFF) + (cpu->BC & 0x0FFF)) > 0x0FFF);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x0A: {
        printf("LD A, (BC)\n");
        cpu->A  = mmu->read8(cpu->BC);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x0B: {
        cpu->BC = cpu->BC - 1;
        printf("DEC BC -- %X --\n", cpu->BC);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x0C: {
        inc(cpu->C);
        printf("INC C -- %X --\n", cpu->C);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x0D: {
        dec(cpu->C);
        printf("DEC C -- %X --\n", cpu->C);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x0E: {
        cpu->C = mmu->romData[cpu->PC + 1];
        printf("LD C, d8 -- %X --\n", cpu->C);
        cpu->PC = cpu->PC + 2;
        break;
    }
    case 0x0F: {
        rrca(cpu->A);
        printf("RRCA --%X --", cpu->A);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x10: {
        // this one should be checked out
        printf("STOP 0x10 \n");
        cpu->PC = cpu->PC + 2;
        break;
    }
    case 0x11: {
        ldr(cpu->DE);
        printf("LD (DE), d16 -- %X --\n", cpu->DE);
        cpu->PC = cpu->PC + 3;
        break;
    }
    case 0x12: {
        printf("LD (DE), A -- %X --\n", cpu->DE);
        mmu->write8(cpu->DE, cpu->A);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x13: {
        cpu->DE = cpu->DE + 1;
        printf("INC DE -- %X --\n", cpu->DE);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x14: {
        inc(cpu->D);
        printf("INC D, -- %X --\n", cpu->D);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x15: {
        dec(cpu->D);
        printf("DEC D -- %X --\n", cpu->D);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x16: {
        cpu->D = mmu->romData[cpu->PC + 1];
        printf("LD D, d8 -- %X --\n", cpu->D);
        cpu->PC = cpu->PC + 2;
        break;
    }
    case 0x17: {
        printf("RLA\n");
        rla();
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x18: {
        int8_t offset;
        offset = static_cast<int8_t>(mmu->romData[cpu->PC + 1]);
        cpu->PC += offset;
        printf("JR r8 -- %X --\n", offset);
        cpu->PC += 2;
        break;
    }
    case 0x19: {
        add16(cpu->HL, cpu->DE);
        cpu->PC = cpu->PC + 1;
        printf("ADD HL, DE --%X --\n", cpu->HL);
        break;
    }
    case 0x1A: {
        cpu->A = mmu->read8(cpu->DE);
        printf("LD A, (DE) -- %X --\n", cpu->A);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x1B: {
        cpu->DE = cpu->DE - 1;
        printf("DEC DE -- %X --\n", cpu->DE);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x1C: {
        inc(cpu->E);
        printf("INC E -- %X --\n", cpu->E);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x1D: {
        dec(cpu->E);
        printf("DEC E -- %X --\n", cpu->E);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x1E: {
        printf("LD E, d8");
        cpu->E  = mmu->romData[cpu->PC + 1];
        cpu->PC = cpu->PC + 2;
        break;
    }
    case 0x1F: {
        printf("RRA\n");
        rra();
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x20: {
        printf("JR NZ, s8\n");
        int8_t v = (int8_t)(mmu->read8(cpu->PC + 1));
        jump(!(cpu->F & FLAG_ZERO), v);
        break;
    }
    case 0x21: {
        uint8_t l = mmu->read8(cpu->PC + 1);
        uint8_t h = mmu->read8(cpu->PC + 2);

        uint16_t v = (h << 8) | l;
        ld(cpu->HL, v);
        printf("LD HL, d16 0x21 -- (HL == %X) --\n", cpu->HL);
        cpu->PC = cpu->PC + 3;
        break;
    }
    case 0x22: {
        printf("LD (HL+), A");
        ld_mem(cpu->A, cpu->HL);
        cpu->HL += 1;
        cpu->PC += 1;
        break;
    }
    case 0x23: {
        printf("INC HL 0x23\n");
        cpu->HL = cpu->HL + 1;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x24: {
        printf("INC H\n");
        inc(cpu->HL);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x25: {
        printf("DEC H");
        dec(cpu->H);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x26: {
        printf("LD H, d8");
        ld(cpu->H, mmu->read8(cpu->PC + 1));
        cpu->PC = cpu->PC + 2;
        break;
    }
    case 0x27: {
        printf("DAA");
        uint8_t correction = 0;
        if (!(cpu->F & FLAG_SUBTRACT)) {
            if ((cpu->A & 0x0F) > 9 || cpu->F & FLAG_HALF_CARRY) {
                correction |= 0x06;
            }
            if ((cpu->A > 0x99) || cpu->F & FLAG_CARRY) {
                correction |= 0x60;
                cpu->set_flag(FLAG_CARRY, true);
            }
        } else {
            if (cpu->F & FLAG_HALF_CARRY)
                correction |= 0x06;
            if (cpu->F & FLAG_CARRY)
                correction |= 0x60;
        }
        // TODO
        if (cpu->F & FLAG_SUBTRACT)
            cpu->A -= correction;
        else
            cpu->A += correction;
        cpu->set_flag(FLAG_ZERO, cpu->A == 0);
        cpu->clear_flag(FLAG_HALF_CARRY);
        break;
    }
    case 0x28: {

        printf("JR Z, r8 0x28");
        int8_t offset = mmu->read8(cpu->PC + 1);
        jump(cpu->F & FLAG_ZERO, offset);
        // pc shouldn't not be updated here break;
    }
    case 0x29: {
        std::cout << "ADD HL, HL" << std::endl;
        add16(cpu->HL, cpu->HL);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x2A: {
        printf("LD A, (HL+)\n");
        ld_mem(cpu->A, cpu->HL);
        cpu->HL += 1;
        cpu->PC += 1;
        break;
    }
    case 0x2B: {
        printf("DEC HL\n");
        cpu->HL -= 1;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x2C: {
        printf("INC L\n");
        inc(cpu->L);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x2D: {
        printf("DEC L\n");
        dec(cpu->L);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x2E: {
        printf("LD L, d8\n");
        ld_mem(cpu->L, mmu->read8(cpu->PC + 1));
        cpu->PC = cpu->PC + 2;
        break;
    }
    case 0x2F: {
        printf("CPL\n");
        cpl(cpu->A);
        cpu->PC = cpu->PC + 2;
        break;
    }
    case 0x30: {
        printf("JR NC,r8 -- --\n");
        jump(!(cpu->F & FLAG_CARRY), mmu->read8(cpu->PC + 1));
        break;
    }
    case 0x31: {

        printf("LD SP, d16\n");
        ldr(cpu->SP);
        cpu->PC = cpu->PC + 3;
        break;
    }
    case 0x32: {

        printf("LD (HL-), A\n");
        ld_mem(cpu->A, cpu->HL);
        cpu->HL -= 1;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x33: {

        printf("INC SP\n");
        cpu->SP = cpu->SP + 1;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x34: {

        printf("INC (HL) 0x34\n");
        inc_mem(cpu->HL);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x35: {

        dec_mem(cpu->HL);
        printf("DEC (HL) 0x35 0x%X \n", mmu->read8(cpu->HL));
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x36: {

        printf("LD (HL), d8\n");
        mmu->write8(cpu->HL, mmu->read8(cpu->PC + 1));
        cpu->PC = cpu->PC + 2;
        break;
    }
    case 0x37: {

        printf("SCF\n");
        cpu->set_flag(FLAG_CARRY, true);
        cpu->clear_flag(FLAG_SUBTRACT);
        cpu->clear_flag(FLAG_HALF_CARRY);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x38: {

        printf("JR C, r8 0x38\n");
        jump(cpu->F & FLAG_CARRY, mmu->read8(cpu->PC + 1));
        // don't change pc on jumps break;
    }
    case 0x39: {
        printf("ADD HL, SP\n");
        add16(cpu->HL, cpu->SP);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x3A: {
        printf("LD A, (HL-)\n");
        cpu->A  = mmu->read8(cpu->HL);
        cpu->HL = cpu->HL - 1;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x3B: {
        printf("DEC SP\n");
        cpu->SP = cpu->SP - 1;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x3C: {
        printf("INC A\n");
        inc(cpu->A);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x3D: {
        printf("DEC A TODO\n");
        dec(cpu->A);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x3E: {

        cpu->A = mmu->read8(cpu->PC + 1);
        printf("LD A, d8: 0x3E A: -- %X -- \n", cpu->A);
        cpu->PC = cpu->PC + 2;
        break;
    }
    case 0x3F: {

        printf("CCF\n");
        bool carry = cpu->F & FLAG_CARRY;
        cpu->set_flag(FLAG_CARRY, !carry);
        cpu->set_flag(FLAG_SUBTRACT, false);
        cpu->set_flag(FLAG_HALF_CARRY, false);
        break;
    }
    case 0x40: {

        printf("CCFLD B, B\n");
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x41: {
        printf("LD B, C\n");
        cpu->B  = cpu->C;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x42: {
        printf("LD B, D\n");
        cpu->B  = cpu->D;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x43: {
        printf("LD B, E\n");
        cpu->B  = cpu->E;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x44: {
        printf("LD B, H\n");
        cpu->B  = cpu->H;
        cpu->PC = cpu->PC + 2;
        break;
    }
    case 0x45: {
        printf("LD B, L\n");
        cpu->B  = cpu->L;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x46: {
        printf("LD B, (HL)");
        cpu->B  = mmu->read16(cpu->HL);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x47: {
        printf("LD B, A\n");
        cpu->B  = cpu->A;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x48: {
        printf("LD C, B\n");
        cpu->C  = cpu->B;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x49: {
        printf("LD C, C\n");
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x4A: {
        printf("LD C, D\n");
        cpu->C  = cpu->C;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x4B: {
        printf("LD C, E\n");
        cpu->C  = cpu->E;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x4C: {
        printf("LD C, H\n");
        cpu->C  = cpu->H;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x4D: {
        printf("LD C, L\n");
        cpu->C  = cpu->L;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x4E: {
        printf("LD C, (HL)\n");
        cpu->C  = mmu->read8(cpu->HL);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x4F: {
        printf("LD C, A\n");
        cpu->C  = cpu->A;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x50: {
        printf("LD B, D\n");
        cpu->B  = cpu->D;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x51: {
        printf("LD D, C\n");
        cpu->B  = cpu->C;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x52: {
        printf("LD C, A\n");
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x53: {
        printf("LD D, E\n");
        cpu->D  = cpu->E;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x54: {
        printf("LD D, H\n");
        cpu->D  = cpu->H;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x55: {
        printf("LD D, L\n");
        cpu->D  = cpu->L;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x56: {
        printf("LD D, (HL)\n");

        cpu->D  = mmu->read8(cpu->HL);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x57: {
        printf("LD D, A\n");
        cpu->D  = cpu->A;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x58: {
        printf("LD D, B\n");
        cpu->D  = cpu->B;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x59: {
        printf("LD E, C\n");
        cpu->E  = cpu->C;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x5A: {
        printf("LD E, D\n");
        cpu->E  = cpu->D;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x5B: {
        printf("LD D, B\n");
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x5C: {
        printf("LD E, H\n");
        cpu->E  = cpu->H;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x5D: {
        printf("LD E, L\n");
        cpu->E  = cpu->L;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x5E: {
        printf("LD E, L\n");
        cpu->E  = mmu->read8(cpu->HL);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x5F: {
        printf("LD E, A\n");
        cpu->E  = cpu->A;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x60: {
        printf("LD H, B\n");
        cpu->H  = cpu->B;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x61: {
        printf("LD H, C\n");
        cpu->H  = cpu->C;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x62: {
        printf("LD H, D\n");
        cpu->H  = cpu->D;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x63: {
        printf("LD H, E\n");
        cpu->H  = cpu->E;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x64: {
        printf("LD H, H\n");
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x65: {
        printf("LD H, L\n");
        cpu->H  = cpu->L;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x66: {
        printf("LD H, (HL)\n");
        cpu->H  = mmu->read8(cpu->HL);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x67: {
        printf("LD H, A\n");
        cpu->H  = cpu->A;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x68: {
        printf("LD L, B\n");
        cpu->L  = cpu->B;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x69: {
        printf("LD L, C\n");
        cpu->L  = cpu->C;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x6A: {
        printf("LD L, D\n");
        cpu->L  = cpu->D;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x6B: {
        printf("LD L, E\n");
        cpu->L  = cpu->E;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x6C: {
        printf("LD L, H\n");
        cpu->L  = cpu->H;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x6D: {
        printf("LD L, L\n");
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x6E: {
        printf("LD L, (LH)\n");
        cpu->L  = mmu->read8(cpu->HL);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x6F: {
        printf("LD L, A\n");
        cpu->L  = cpu->A;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x70: {
        printf("LD (HL), B\n");
        mmu->write8(cpu->HL, cpu->B);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x71: {
        printf("LD (HL), C\n");
        mmu->write8(cpu->HL, cpu->C);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x72: {
        printf("LD (HL), D\n");
        mmu->write8(cpu->HL, cpu->D);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x73: {
        printf("LD (HL), E\n");
        mmu->write8(cpu->HL, cpu->E);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x74: {
        printf("LD (HL), H\n");
        mmu->write8(cpu->HL, cpu->H);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x75: {
        printf("LD (HL), L\n");
        mmu->write8(cpu->HL, cpu->L);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x76: {
        cpu->halted = true;
        cpu->PC     = cpu->PC + 1;
        break;
    }
    case 0x77: {
        printf("LD (HL), A\n");
        mmu->write8(cpu->HL, cpu->A);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x78: {
        printf("LD A, B\n");
        cpu->A  = cpu->B;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x79: {
        printf("LD A, C\n");
        cpu->A  = cpu->C;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x7A: {
        printf("LD A, D\n");
        cpu->A  = cpu->D;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x7B: {
        printf("LD A, E\n");
        cpu->A  = cpu->E;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x7C: {
        printf("LD A, H\n");
        cpu->A  = cpu->H;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x7D: {
        printf("LD A, L\n");
        cpu->A  = cpu->L;
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x7E: {
        printf("LD A, (HL)\n");
        cpu->A  = mmu->read8(cpu->HL);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x7F: {
        printf("LD A, A\n");
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x80: {
        printf("ADD A, B\n");
        add8(cpu->A, cpu->B);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x81: {
        printf("ADD A, C\n");
        add8(cpu->A, cpu->C);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x82: {
        printf("ADD A, D\n");
        add8(cpu->A, cpu->D);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x83: {
        printf("ADD A, E\n");
        add8(cpu->A, cpu->E);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x84: {
        printf("ADD A, H\n");
        add8(cpu->A, cpu->H);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x85: {
        printf("ADD A, L\n");
        add8(cpu->A, cpu->L);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x86: {
        printf("ADD A, (HL)\n");
        add8_mem(cpu->A, mmu->romData[cpu->HL]);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x87: {
        printf("ADD A, A\n");
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x88: {
        printf("ADD A, B\n");
        adc(cpu->A, cpu->B);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x89: {
        printf("ADC A, C\n");
        adc(cpu->A, cpu->C);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x8A: {
        printf("ADC A, D\n");
        adc(cpu->A, cpu->D);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x8B: {
        printf("ADC A, E\n");
        adc(cpu->A, cpu->E);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x8C: {
        printf("ADC A, H\n");
        adc(cpu->A, cpu->H);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x8D: {
        printf("ADC A, L\n");
        adc(cpu->A, cpu->L);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x8E: {
        printf("ADC A, (HL)\n");
        adc(cpu->A, mmu->read8(cpu->L));
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x8F: {
        printf("ADC A, L\n");
        adc(cpu->A, cpu->A);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x90: {
        printf("SUB A, L\n");
        sub(cpu->A, cpu->B);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x91: {
        printf("SUB C\n");
        sub(cpu->A, cpu->C);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x92: {
        printf("SUB D\n");
        sub(cpu->A, cpu->D);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x93: {
        printf("SUB E\n");
        sub(cpu->A, cpu->E);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x94: {
        printf("SUB H\n");
        sub(cpu->A, cpu->H);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x95: {
        printf("SUB L\n");
        sub(cpu->A, cpu->L);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x96: {
        printf("SUB (HL)\n");
        sub(cpu->A, cpu->HL);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x97: {
        printf("SUB A\n");
        sub(cpu->A, cpu->A);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x98: {
        printf("SBC A, B\n");
        sbc(cpu->A, cpu->B);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x99: {
        printf("SBC A, C\n");
        sbc(cpu->A, cpu->C);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x9A: {
        printf("SBC A, D\n");
        sbc(cpu->A, cpu->D);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x9B: {
        printf("SBC A, E\n");
        sbc(cpu->A, cpu->E);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x9C: {
        printf("SBC A, H\n");
        sbc(cpu->A, cpu->H);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x9D: {
        printf("SBC A, L\n");
        sbc(cpu->A, cpu->L);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x9E: {
        printf("SBC A, L\n");
        sbc(cpu->A, mmu->read8(cpu->HL));
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0x9F: {
        printf("SBC A, A\n");
        sbc(cpu->A, cpu->A);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xA0: {
        printf("AND A, B\n");
        and_(cpu->A, cpu->B);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xA1: {
        printf("AND A, C\n");
        and_(cpu->A, cpu->C);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xA2: {
        printf("AND A, C\n");
        and_(cpu->A, cpu->D);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xA3: {
        printf("AND A, E\n");
        and_(cpu->A, cpu->E);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xA4: {
        printf("AND A, H\n");
        and_(cpu->A, cpu->H);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xA5: {
        printf("AND A, L\n");
        and_(cpu->A, cpu->L);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xA6: {
        printf("AND A, (HL)\n");
        and_(cpu->A, mmu->read8(cpu->L));
        cpu->PC = cpu->PC + 1;
        break;
    }

    case 0xA7: {
        printf("AND A, (HL)\n");
        and_(cpu->A, mmu->read8(cpu->L));
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xA8: {
        printf("XOR A, B\n");
        xor_(cpu->A, cpu->B);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xA9: {
        printf("XOR A, C\n");
        xor_(cpu->A, cpu->C);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xAA: {
        printf("XOR A, D\n");
        xor_(cpu->A, cpu->D);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xAB: {
        printf("XOR A, E\n");
        xor_(cpu->A, cpu->E);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xAC: {
        printf("XOR A, H\n");
        xor_(cpu->A, cpu->H);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xAD: {
        printf("XOR A, L\n");
        xor_(cpu->A, cpu->L);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xAE: {
        printf("XOR A, (HL)\n");
        xor_(cpu->A, mmu->read8(cpu->HL));
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xAF: {
        printf("XOR A, A\n");
        xor_(cpu->A, cpu->A);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xB0: {
        printf("OR A, B\n");
        or_(cpu->A, cpu->B);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xB1: {
        printf("OR A, C\n");
        or_(cpu->A, cpu->C);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xB2: {
        printf("OR A, D\n");
        or_(cpu->A, cpu->D);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xB3: {
        printf("OR A, E\n");
        or_(cpu->A, cpu->E);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xB4: {
        printf("OR A, H\n");
        or_(cpu->A, cpu->H);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xB5: {
        printf("OR A, L\n");
        or_(cpu->A, cpu->L);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xB6: {
        printf("OR A, (HL)\n");
        or_(cpu->A, mmu->read8(cpu->HL));
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xB7: {
        printf("OR A, A\n");
        or_(cpu->A, cpu->A);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xB8: {
        printf("CP B\n");
        cp_(cpu->A, cpu->B);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xB9: {
        printf("CP C\n");
        cp_(cpu->A, cpu->C);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xBA: {
        printf("CP D\n");
        cp_(cpu->A, cpu->D);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xBB: {
        printf("CP E\n");
        cp_(cpu->A, cpu->E);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xBC: {
        printf("CP H\n");
        cp_(cpu->A, cpu->H);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xBD: {
        printf("CP L\n");
        cp_(cpu->A, cpu->L);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xBE: {
        printf("CP (HL)\n");
        cp_(cpu->A, mmu->read8(cpu->HL));
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xBF: {
        printf("CP A\n");
        cp_(cpu->A, cpu->A);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xC0: {
        printf("RET NZ\n");
        if (!(cpu->F & FLAG_ZERO)) {
            ret(true);
        } else {
            cpu->PC = cpu->PC + 1;
        }
        break;
    }
    case 0xC1: {
        printf("POP BC\n");
        cpu->C = mmu->read8(cpu->SP);
        cpu->B = mmu->read8(cpu->SP + 1);
        cpu->SP += 1;
        break;
    }
    case 0xC2: {
        printf("JP NZ, nn\n");
        if (!(cpu->F & FLAG_ZERO)) {
            cpu->PC = mmu->read8(cpu->PC) | mmu->read8(cpu->PC + 1) << 8;
        } else {
            cpu->PC += 3;
        }
        break;
    }
    case 0xC3: {
        uint8_t l = mmu->read8(cpu->PC + 1);
        uint8_t h = mmu->read8(cpu->PC + 2);
        cpu->PC   = (h << 8) | l;
        printf("JP a16 -- %X --\n", cpu->PC);
        break;
    }
    case 0xC4: {
        printf("CALL nn 0xc4");
        call(!(cpu->F & FLAG_ZERO));
        break;
    }
    case 0xC5: {
        printf("PUSH BC\n");
        push_(cpu->BC);
        cpu->PC += 1;
        break;
    }
    case 0xC6: {
        printf("ADD A, d8\n");
        add8(cpu->A, mmu->read8(cpu->PC + 1));
        cpu->PC = cpu->PC + 2;
        break;
    }
    case 0xC7: {
        printf("RST 0\n");
        uint16_t ret = cpu->PC + 1;
        cpu->SP -= 1;
        mmu->write8(cpu->SP, (ret >> 8) & 0xFF);
        cpu->SP -= 1;
        mmu->write8(cpu->SP, ret & 0xFF);
        cpu->PC = cpu->PC + 1;
        cpu->PC = 0x0000;
        break;
    }
    case 0xC8: {
        printf("RET Z\n");
        if (cpu->F & FLAG_ZERO) {
            ret(true);
        } else {
            cpu->PC += 1;
        }
        break;
    }
    case 0xC9: {
        printf("RET\n");
        pop_(true, cpu->PC);
        break;
    }
    case 0xCA: {
        printf("JP Z, nn 0xCA\n");
        if (cpu->F & FLAG_ZERO) {
            u_int8_t l = mmu->read8(cpu->PC + 1);
            u_int8_t h = mmu->read8(cpu->PC + 2);
            cpu->PC    = (h << 8) | l;
        } else {
            cpu->PC += 3;
        }
        break;
    }
    case 0xCB: {
        printf("--- PREFIX CB NOT 0xCB -- \n");
        switch (mmu->read8(cpu->PC + 1)) {
        case 0x00: {
            printf("RLC B\n");
            rlc_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0x01: {
            printf("RLC c\n");
            rlc_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0x02: {
            printf("rlc d\n");
            rlc_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0x03: {
            printf("RLC E\n");
            rlc_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0x04: {
            printf("RLC H\n");
            rlc_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0x05: {
            printf("RLC L\n");
            rlc_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0x06: {
            printf("RLC (HL)\n");
            uint8_t v = mmu->read8(cpu->HL);
            rlc_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0x07: {
            printf("RLC A\n");
            rlc_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0x08: {
            printf("RRC B\n");
            rrc_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0x09: {
            printf("RRC C\n");
            rrc_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0x0A: {
            printf("RRC D\n");
            rrc_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0x0B: {
            printf("RRC E\n");
            rrc_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0x0C: {
            printf("RRC H\n");
            rrc_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0x0D: {
            printf("RRC L\n");
            rrc_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0x0E: {
            printf("RRC (HL)\n");
            uint8_t v = mmu->read8(cpu->HL);
            rrc_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0x0F: {
            printf("RL B\n");
            rrc_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0x10: {
            printf("RL B \n");
            rl_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0x11: {
            printf("RL C\n");
            rl_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0x12: {
            printf("RL D\n");
            rl_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0x13: {
            printf("RL E\n");
            rl_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0x14: {
            printf("RL H\n");
            rl_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0x15: {
            printf("RL L \n");
            rl_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0x16: {
            printf("RL (HL)");
            uint8_t v = mmu->read8(cpu->HL);
            rl_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0x17: {
            printf("RL A\n");
            rl_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0x18: {
            printf("RR B\n");
            rr_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0x19: {
            printf("RR B\n");
            rr_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0x1A: {
            printf("RR D\n");
            rr_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0x1B: {
            printf("RR D\n");
            rr_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0x1C: {
            printf("RR H\n");
            rr_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0x1D: {
            printf("RR L\n");
            rr_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0x1E: {
            printf("RR L\n");
            uint8_t v = mmu->read8(cpu->HL);
            rr_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0x1F: {
            printf("RR A\n");
            rr_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0x20: {
            printf("SLA B\n");
            sla_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0x21: {
            printf("SLA C\n");
            sla_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0x22: {
            printf("SLA D\n");
            sla_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0x23: {
            printf("SLA E\n");
            sla_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0x24: {
            printf("SLA H\n");
            sla_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0x25: {
            printf("SLA L\n");
            sla_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0x26: {
            printf("SLA (HL)\n");
            uint8_t v = mmu->read8(cpu->HL);
            sla_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0x27: {
            printf("SLA A\n");
            sla_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0x28: {
            printf("SLA B\n");
            sra_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0x29: {
            printf("SRA C\n");
            sra_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0x2A: {
            printf("SRA C\n");
            sra_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0x2B: {
            printf("SRA D\n");
            sra_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0x2C: {
            printf("SRA E\n");
            sra_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0x2D: {
            printf("SRA L\n");
            sra_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0x2E: {
            printf("SRA (HL)\n");
            uint8_t v = mmu->read8(cpu->HL);
            sra_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0x2F: {
            printf("SRA A\n");
            sra_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0x30: {
            printf("SWAP B\n");
            swap_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0x31: {
            printf("SWAP C\n");
            swap_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0x32: {
            printf("SWAP D\n");
            swap_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0x33: {
            printf("SWAP E\n");
            swap_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0x34: {
            printf("SWAP H\n");
            swap_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0x35: {
            printf("SWAP L\n");
            swap_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0x36: {
            printf("SWAP (HL)\n");
            uint8_t v = mmu->read8(cpu->HL);
            swap_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0x37: {
            printf("SWAP A\n");
            swap_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0x38: {
            printf("SRL B\n");
            srl_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0x39: {
            printf("SRL C\n");
            srl_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0x3A: {
            printf("SRL D\n");
            srl_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0x3B: {
            printf("SRL E\n");
            srl_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0x3C: {
            printf("SRL H\n");
            srl_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0x3D: {
            printf("SRL L\n");
            srl_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0x3E: {
            printf("SRL (HL)\n");
            uint8_t v = mmu->read8(cpu->HL);
            srl_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0x3F: {
            printf("SRL A\n");
            srl_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0x40: {
            printf("BIT0 B\n");
            bit0_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0x41: {
            printf("BIT0 C\n");
            bit0_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0x42: {
            printf("BIT0 D\n");
            bit0_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0x43: {
            printf("BIT0 E\n");
            bit0_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0x44: {
            printf("BIT0 H\n");
            bit0_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0x45: {
            printf("BIT0 L\n");
            bit0_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0x46: {
            printf("BIT0 (HL)\n");
            uint8_t v = mmu->read8(cpu->HL);
            bit0_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0x47: {
            printf("BIT0 A\n");
            bit0_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0x48: {
            printf("BIT1 B\n");
            bit1_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0x49: {
            printf("BIT1 C\n");
            bit1_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0x4A: {
            printf("BIT1 D\n");
            bit1_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0x4B: {
            printf("BIT1 E\n");
            bit1_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0x4C: {
            printf("BIT1 H\n");
            bit1_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0x4D: {
            printf("BIT1 L\n");
            bit1_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0x4E: {
            uint8_t v = mmu->read8(cpu->HL);
            printf("BIT1 (HL)\n");
            bit1_extended(v);
            mmu->write8(cpu->HL, v);
            break;
        }
        case 0x4F: {
            printf("BIT1 A\n");
            bit1_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0x50: {
            printf("BIT2, B\n");
            bit2_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0x51: {
            printf("BIT2, C\n");
            bit2_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0x52: {
            printf("BIT2, D\n");
            bit2_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0x53: {
            printf("BIT2, E\n");
            bit2_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0x54: {
            printf("BIT2, H\n");
            bit2_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0x55: {
            printf("BIT2, L\n");
            bit2_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0x56: {
            printf("BIT2, L\n");
            uint8_t v = mmu->read8(cpu->HL);
            bit2_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0x57: {
            printf("BIT2 A\n");
            bit2_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0x58: {
            printf("BIT3 B\n");
            bit3_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0x59: {
            printf("BIT3 C\n");
            bit3_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0x5A: {
            printf("BIT3 D\n");
            bit3_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0x5B: {
            printf("BIT3 E\n");
            bit3_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0x5C: {
            printf("BIT3 H\n");
            bit3_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0x5D: {
            printf("BIT3 L\n");
            bit3_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0x5E: {
            printf("BIT3 L\n");
            uint8_t v = mmu->read8(cpu->HL);
            bit3_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0x5F: {
            printf("BIT3 A\n");
            bit3_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0x60: {
            printf("BIT4 B\n");
            bit4_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0x61: {
            printf("BIT4 C\n");
            bit4_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0x62: {
            printf("BIT4 D\n");
            bit4_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0x63: {
            printf("BIT4 E\n");
            bit4_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0x64: {
            printf("BIT4 H\n");
            bit4_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0x65: {
            printf("BIT4 L\n");
            bit4_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0x66: {
            printf("BIT4 L\n");
            uint8_t v = mmu->read8(cpu->HL);
            bit4_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0x67: {
            printf("BIT4 A\n");
            bit4_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0x68: {
            printf("BIT4 A\n");
            bit4_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0x69: {
            printf("BIT5 B\n");
            bit4_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0x6A: {
            printf("BIT5 C\n");
            bit4_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0x6B: {
            printf("BIT5 D\n");
            bit5_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0x6C: {
            printf("BIT5 E\n");
            bit5_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0x6D: {
            printf("BIT5 L\n");
            bit5_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0x6E: {
            printf("BIT5 L\n");
            uint8_t v = mmu->read8(cpu->HL);
            bit5_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0x6F: {
            printf("BIT5 A\n");
            bit5_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0x70: {
            printf("BIT6 A\n");
            bit6_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0x71: {
            printf("BIT6 C\n");
            bit6_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0x72: {
            printf("BIT6 D\n");
            bit6_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0x73: {
            printf("BIT6 E\n");
            bit6_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0x74: {
            printf("BIT6 H\n");
            bit6_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0x75: {
            printf("BIT6 L\n");
            bit6_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0x76: {
            printf("BIT6 L\n");
            uint8_t v = mmu->read8(cpu->HL);
            bit6_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0x77: {
            printf("BIT6 A\n");
            bit6_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0x78: {
            printf("BIT7 A\n");
            bit7_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0x79: {
            printf("BIT7 C\n");
            bit7_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0x7A: {
            printf("BIT7 D\n");
            bit7_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0x7B: {
            printf("BIT7 E\n");
            bit7_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0x7C: {
            printf("BIT7 H\n");
            bit7_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0x7D: {
            printf("BIT7 L\n");
            bit7_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0x7E: {
            printf("BIT7 L\n");
            uint8_t v = mmu->read8(cpu->HL);
            bit7_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
        }
        case 0x7F: {
            printf("BIT7 A\n");
            bit7_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0x80: {
            printf("RES0 B\n");
            res0_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0x81: {
            printf("RES0 C\n");
            res0_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0x82: {
            printf("RES0 D\n");
            res0_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0x83: {
            printf("RES0 E\n");
            res0_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0x84: {
            printf("RES0 H\n");
            res0_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0x85: {
            printf("RES0 L\n");
            res0_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0x86: {
            printf("RES0 (HL)\n");
            uint8_t v = mmu->read8(cpu->HL);
            res0_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0x87: {
            printf("RES0 L\n");
            res0_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0x88: {
            printf("RES1 B\n");
            res1_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0x89: {
            printf("RES1 C\n");
            res1_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0x8A: {
            printf("RES1 D\n");
            res1_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0x8B: {
            printf("RES1 E\n");
            res1_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0x8C: {
            printf("RES1 E\n");
            res1_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0x8D: {
            printf("RES1 L\n");
            res1_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0x8E: {
            printf("RES1 (HL)\n");
            uint8_t v = mmu->read8(cpu->HL);
            res1_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0x8F: {
            printf("RES1 A\n");
            res1_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0x90: {
            printf("RES2 B\n");
            res2_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0x91: {
            printf("RES2 C\n");
            res2_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0x92: {
            printf("RES2 D\n");
            res2_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0x93: {
            printf("RES2 E\n");
            res2_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0x94: {
            printf("RES2 H\n");
            res2_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0x95: {
            printf("RES2 L\n");
            res2_extended(cpu->L);
            cpu->PC += 2;
            break;
        }

        case 0x96: {
            printf("RES2 L\n");
            uint8_t v = mmu->read8(cpu->HL);
            res2_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0x97: {
            printf("RES2 A\n");
            res2_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0x98: {
            printf("RES3 B\n");
            res3_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0x99: {
            printf("RES3 C\n");
            res3_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0x9A: {
            printf("RES3 D\n");
            res3_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0x9B: {
            printf("RES3 E\n");
            res3_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0x9C: {
            printf("RES3 H\n");
            res3_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0x9D: {
            printf("RES3 L\n");
            res3_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0x9E: {
            printf("RES3 L\n");
            uint8_t v = mmu->read8(cpu->HL);
            res3_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0x9F: {
            printf("RES3 A\n");
            res3_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0xA0: {
            printf("RES4 A\n");
            res4_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0xA1: {
            printf("RES4 C\n");
            res4_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0xA2: {
            printf("RES4 D\n");
            res4_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0xA3: {
            printf("RES4 E\n");
            res4_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0xA4: {
            printf("RES4 H\n");
            res4_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0xA5: {
            printf("RES4 L\n");
            res4_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0xA6: {
            printf("RES4 L\n");
            uint8_t v = mmu->read8(cpu->HL);
            res4_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0xA7: {
            printf("RES4 A\n");
            res4_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0xA8: {
            printf("RES5 B\n");
            res5_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0xA9: {
            printf("RES5 C\n");
            res5_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0xAA: {
            printf("RES5 D\n");
            res5_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0xAB: {
            printf("RES5 E\n");
            res5_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0xAC: {
            printf("RES5 H\n");
            res5_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0xAD: {
            printf("RES5 L\n");
            res5_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0xAE: {
            printf("RES5 (HL)\n");
            uint8_t v = mmu->read8(cpu->HL);
            res5_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0xAF: {
            printf("RES5 A\n");
            res5_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0xB0: {
            printf("RES6 B\n");
            res6_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0xB1: {
            printf("RES6 C\n");
            res6_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0xB2: {
            printf("RES6 D\n");
            res6_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0xB3: {
            printf("RES6 E\n");
            res6_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0xB4: {
            printf("RES6 H\n");
            res6_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0xB5: {
            printf("RES6 L\n");
            res6_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0xB6: {
            printf("RES6 (HL)\n");
            uint8_t v = mmu->read8(cpu->HL);
            res6_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0xB7: {
            printf("RES6 A\n");
            res6_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0xB8: {
            printf("RES7 B\n");
            res7_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0xB9: {
            printf("RES7 C\n");
            res7_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0xBA: {
            printf("RES7 D\n");
            res7_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0xBB: {
            printf("RES7 E\n");
            res7_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0xBC: {
            printf("RES7 H\n");
            res7_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0xBD: {
            printf("RES7 L\n");
            res7_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0xBE: {
            printf("RES7 (HL)\n");
            uint8_t v = mmu->read8(cpu->HL);
            res7_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0xBF: {
            printf("RES7 A\n");
            res7_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0xC0: {
            printf("SET0 B\n");
            set0_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0xC1: {
            printf("SET0 C\n");
            set0_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0xC2: {
            printf("SET0 D\n");
            set0_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0xC3: {
            printf("SET0 E\n");
            set0_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0xC4: {
            printf("SET0 H\n");
            set0_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0xC5: {
            printf("SET0 L\n");
            set0_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0xC6: {
            printf("SET0 L\n");
            uint8_t v = mmu->read8(cpu->HL);
            set0_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0xC7: {
            printf("SET0 A\n");
            set0_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0xC8: {
            printf("SET1 B\n");
            set1_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0xC9: {
            printf("SET1 C\n");
            set1_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0xCA: {
            printf("SET1 D\n");
            set1_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0xCB: {
            printf("SET1 E\n");
            set1_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0xCC: {
            printf("SET1 H\n");
            set1_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0xCD: {
            printf("SET1 L\n");
            set1_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0xCE: {
            printf("SET1 (HL)\n");
            uint8_t v = mmu->read8(cpu->HL);
            set1_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0xCF: {
            printf("SET1 A\n");
            set1_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0xD0: {
            printf("SET2 B\n");
            set2_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0xD1: {
            printf("SET2 C\n");
            set2_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0xD2: {
            printf("SET2 D\n");
            set2_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0xD3: {
            printf("SET2 E\n");
            set2_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0xD4: {
            printf("SET2 D\n");
            set2_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0xD5: {
            printf("SET2 L\n");
            set2_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0xD6: {
            printf("SET2 L\n");
            uint8_t v = mmu->read8(cpu->HL);
            set2_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0xD7: {
            printf("SET2 A\n");
            set2_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0xD8: {
            printf("SET3 B\n");
            set3_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0xD9: {
            printf("SET3 C\n");
            set3_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0xDA: {
            printf("SET3 D\n");
            set3_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0xDB: {
            printf("SET3 E\n");
            set3_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0xDC: {
            printf("SET3 H\n");
            set3_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0xDD: {
            printf("SET3 L\n");
            set3_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0xDE: {
            printf("SET3 (HL)\n");
            uint8_t v = mmu->read8(cpu->HL);
            set3_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0xDF: {
            printf("SET3 A\n");
            set3_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0xE0: {
            printf("SET4 B\n");
            set4_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0xE1: {
            printf("SET4 C\n");
            set4_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0xE2: {
            printf("SET4 D\n");
            set4_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0xE3: {
            printf("SET4 E\n");
            set4_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0xE4: {
            printf("SET4 H\n");
            set4_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0xE5: {
            printf("SET4 L\n");
            set4_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0xE6: {
            printf("SET4 (HL)\n");
            uint8_t v = mmu->read8(cpu->HL);
            set4_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0xE7: {
            printf("SET4 A\n");
            set4_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0xE8: {
            printf("SET5 B\n");
            set5_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0xE9: {
            printf("SET5 C\n");
            set5_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0xEA: {
            printf("SET5 D\n");
            set5_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0xEB: {
            printf("SET5 E\n");
            set5_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0xEC: {
            printf("SET5 H\n");
            set5_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0xED: {
            printf("SET5 L\n");
            set5_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0xEE: {
            printf("SET5 (HL)\n");
            uint8_t v = mmu->read8(cpu->HL);
            set5_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0xEF: {
            printf("SET5 A\n");
            set5_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0xF0: {
            printf("SET6 B\n");
            set6_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0xF1: {
            printf("SET6 C\n");
            set6_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0xF2: {
            printf("SET6 D\n");
            set6_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0xF3: {
            printf("SET6 E\n");
            set6_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0xF4: {
            printf("SET6 H\n");
            set6_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0xF5: {
            printf("SET6 L\n");
            set6_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0xF6: {
            printf("SET6 (HL)\n");
            uint8_t v = mmu->read8(cpu->HL);
            set6_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0xF7: {
            printf("SET6 A\n");
            set6_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        case 0xF8: {
            printf("SET7 B\n");
            set7_extended(cpu->B);
            cpu->PC += 2;
            break;
        }
        case 0xF9: {
            printf("SET7 C\n");
            set7_extended(cpu->C);
            cpu->PC += 2;
            break;
        }
        case 0xFA: {
            printf("SET7 D\n");
            set7_extended(cpu->D);
            cpu->PC += 2;
            break;
        }
        case 0xFB: {
            printf("SET7 E\n");
            set7_extended(cpu->E);
            cpu->PC += 2;
            break;
        }
        case 0xFC: {
            printf("SET7 H\n");
            set7_extended(cpu->H);
            cpu->PC += 2;
            break;
        }
        case 0xFD: {
            printf("SET7 L\n");
            set7_extended(cpu->L);
            cpu->PC += 2;
            break;
        }
        case 0xFE: {
            printf("SET7 L\n");
            uint8_t v = mmu->read8(cpu->HL);
            set7_extended(v);
            mmu->write8(cpu->HL, v);
            cpu->PC += 2;
            break;
        }
        case 0xFF: {
            printf("SET7 A\n");
            set7_extended(cpu->A);
            cpu->PC += 2;
            break;
        }
        }
    }
    case 0xCC: {
        printf("CALL Z, a16\n");
        if (cpu->F & FLAG_ZERO) {
            execute_call();
        } else {
            cpu->PC = cpu->PC + 3;
        }
        break;
    }
    case 0xCD: {
        printf("CALL a16\n");
        uint16_t address        = mmu->read16(cpu->PC + 1);
        uint16_t return_address = cpu->PC + 3;

        cpu->SP -= 2;
        mmu->write16(cpu->SP, return_address);

        cpu->PC = address;
        break;
    }
    case 0xCE: {
        adc(cpu->A, mmu->read8(cpu->PC + 1));
        cpu->PC = cpu->PC + 2;
        break;
    }
    case 0xCF: {
        printf("RST 1\n");
        uint16_t return_addr = cpu->PC + 1;
        cpu->SP -= 1;
        mmu->write16(cpu->SP, return_addr);
        cpu->PC = cpu->PC + 2;
        break;
    }
    case 0xD0: {
        printf("RET NC\n");
        pop_(!(cpu->F & FLAG_CARRY), cpu->PC);
        break;
    }
    case 0xD1: {
        printf("POP DE\n");
        pop_(true, cpu->DE);
        break;
    }
    case 0xD2: {
        printf("JP NC, a16\n");
        pop_(!(cpu->F & FLAG_CARRY), cpu->PC);
        break;
    }
    case 0xD3: {
        printf("-- NOT IMPLEMENTED --\n");
        // cpu->PC = cpu->PC + 2;
        break;
    }
    case 0xD4: {
        printf("  CALL NC, a16\n");
        call((cpu->F & FLAG_CARRY));
        break;
    }
    case 0xD5: {
        printf(" PUSH DE\n");
        push_(cpu->DE);
        cpu->PC += 1;
        break;
    }
    case 0xD6: {
        sub(cpu->A, mmu->romData[cpu->PC + 1]);
        printf("SUB d8. result of A -- %X -- 0xD6\n", cpu->A);
        cpu->PC = cpu->PC + 2;
        break;
    }
    case 0xD7: {
        printf("RST 10H\n");
        uint16_t ret = cpu->PC + 1;
        cpu->SP--;
        mmu->write8(cpu->SP, (ret >> 8) & 0xFF);
        cpu->SP--;
        mmu->write8(cpu->SP, ret & 0xFF);
        cpu->PC = 0x0010;
        break;
    }
    case 0xD8: {
        if (cpu->F & FLAG_CARRY) {
            ret(true);
        } else {
            cpu->PC += 1;
        }
        return;
    }
    case 0xD9: {
        // TODO
        printf("-- 0xD9 -- RETI NOT DONE \n");
        // pop_();
        // cpu->PC = cpu->PC + 2;
        // break;
    }
    case 0xDA: {
        printf("JP C, nn\n");
        if (cpu->F & FLAG_CARRY) {
            uint8_t l = mmu->read8(cpu->PC + 1);
            uint8_t h = mmu->read8(cpu->PC + 2);
            cpu->PC   = (h << 8) | l;
        } else {
            cpu->PC = cpu->PC + 3;
        }
        break;
    }
    case 0xDB: {
        printf("-- NOT IMPLEMENTED --\n");
        // cpu->PC = cpu->PC + 2;
        break;
    }

    case 0xDC: {
        printf("CALL C, a16\n");
        if (cpu->F & FLAG_CARRY) {
            uint16_t return_addr = cpu->PC + 3;
            cpu->SP -= 1;
            mmu->write8(cpu->SP, (return_addr >> 8));
            cpu->SP -= 1;
            mmu->write8(cpu->SP, return_addr & 0xFF);
            cpu->PC = return_addr;
        } else {
            cpu->PC = cpu->PC + 3;
        }
        break;
    }
    case 0xDD: {
        printf("-- NOT IMPLEMENTED --\n");
        // cpu->PC = cpu->PC + 2;
        break;
    }
    case 0xDE: {
        printf("SBC A, d8\n");
        sbc(cpu->A, mmu->read8(cpu->PC + 1));
        cpu->PC = cpu->PC + 2;
        break;
    }
    case 0xDF: {
        printf("RST 18H\n");
        uint16_t return_addr = cpu->PC + 1;

        cpu->SP -= 1;
        mmu->write8(cpu->SP, (return_addr >> 8) & 0xFF);

        cpu->SP -= 1;
        mmu->write8(cpu->SP, return_addr & 0xFF);

        cpu->PC = 0x0018;
        break;
    }
    case 0xE0: {
        uint8_t a8 = mmu->read8(cpu->PC + 1);

        mmu->write8(0xFF00 + a8, cpu->A);

        cpu->PC += 2;
        break;
    }
    case 0xE1: {
        printf("POP HL\n");
        pop_(true, cpu->HL);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xE2: {
        uint8_t a8 = mmu->read8(cpu->PC + 1);
        mmu->write8(0xFF00 + a8, cpu->C);
        cpu->PC += 1;
        break;
    }
    case 0xE3: {
        printf("-- NOT IMPLEMENTED --\n");
        break;
    }
    case 0xE4: {
        printf("-- NOT IMPLEMENTED --\n");
        break;
    }
    case 0xE5: {
        printf("PUSH HL\n");
        push_(cpu->HL);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xE6: {
        printf(" AND d8\n");
        and_(cpu->A, mmu->read8(cpu->PC + 1));
        cpu->PC = cpu->PC + 2;
        break;
    }
    case 0xE7: {
        printf("RST 4\n");
        uint16_t n8 = cpu->PC + 1;
        push_(n8);
        cpu->PC = 0x0020;
        break;
    }
    case 0xE8: {
        printf("ADD SP, r8\n");
        int8_t s8 = (int8_t)mmu->read8(cpu->PC + 1);
        cpu->clear_flag(FLAG_ZERO);
        cpu->clear_flag(FLAG_SUBTRACT);
        uint16_t old_sp = cpu->SP;
        uint16_t result = old_sp + s8;
        cpu->set_flag(FLAG_HALF_CARRY, ((old_sp & 0xF) + (s8 & 0xF)) > 0xF);
        cpu->set_flag(FLAG_CARRY, ((old_sp & 0xFF) + (s8 & 0xFF)) > 0xFF);
        cpu->SP = result;

        cpu->PC = cpu->PC + 2;
        break;
    }
    case 0xE9: {
        printf("JP (HL)\n");
        cpu->PC = cpu->HL;
        break;
    }
    case 0xEA: {
        uint8_t  l  = mmu->romData[cpu->PC + 1];
        uint8_t  h  = mmu->romData[cpu->PC + 2];
        uint16_t nn = (h << 8) | l;
        printf("LD(nn), A 0xEA nn: -- %X --\n", nn);
        mmu->write8(nn, cpu->A);
        cpu->PC = cpu->PC + 3;
        break;
    }
    case 0xEB: {
        printf("-- NOT IMPLEMENTED --\n");
        // cpu->PC = cpu->PC + 3;
        break;
    }
    case 0xEC: {
        printf("-- NOT IMPLEMENTED --\n");
        // cpu->PC = cpu->PC + 3;
        break;
    }
    case 0xED: {
        printf("-- NOT IMPLEMENTED --\n");
        // cpu->PC = cpu->PC + 3;
        break;
    }
    case 0xEE: {
        printf(" XOR d8\n");
        xor_(cpu->A, mmu->read8(cpu->PC + 1));
        cpu->PC = cpu->PC + 2;
        break;
    }
    case 0xEF: {
        printf(" RST 28H\n");
        uint16_t return_addr = cpu->PC + 1;
        push_(return_addr);
        cpu->PC = 0x0028;
        break;
    }
    case 0xF0: {
        printf("LD A, (a8)\n");
        uint8_t a8 = mmu->read8(cpu->PC + 1);
        cpu->A     = mmu->read8(0xFF00 + a8);
        cpu->PC    = cpu->PC + 2;
        break;
    }
    case 0xF1: {
        printf("POP AF\n");
        pop_(true, cpu->AF);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xF2: {
        printf("LDA, (C)\n");
        cpu->A  = mmu->read8(0xFF00 + cpu->C);
        cpu->PC = cpu->PC + 1;
        break;
    }
    case 0xF3: {
        printf(" DI\n");
        cpu->_ime = 0;
        cpu->PC += 1;
    }
    case 0xF4: {
        printf("-- NOT IMPLEMENTED --\n");
        // cpu->PC = cpu->PC + 3;
        break;
    }
    case 0xF5: {
        printf("push AF\n");
        // cpu->PC = cpu->PC + 3;
        push_(cpu->AF);
        cpu->PC += 1;
        break;
    }
    case 0xF6: {
        printf(" OR nn\n");
        or_(cpu->A, mmu->read8(cpu->PC + 1));
        cpu->PC = cpu->PC + 2;
        break;
    }
    case 0xF7: {
        printf(" RST 30H\n");
        uint16_t n8 = cpu->PC + 1;
        push_(n8);
        cpu->PC = 0x0030;
        break;
    }
    case 0xF8: {
        printf("LD HL, SP+s8\n");
        int8_t   s8 = mmu->read8(cpu->PC + 1);
        uint32_t r  = cpu->SP + s8;
        cpu->PC     = cpu->PC + 2;
        cpu->HL     = r;
        cpu->clear_flag(FLAG_ZERO);
        cpu->clear_flag(FLAG_SUBTRACT);
        cpu->set_flag(FLAG_HALF_CARRY, (cpu->SP & 0x0F) + (s8 & 0x0F) > 0x0F);
        cpu->set_flag(FLAG_CARRY, r > 0xFF);
        break;
    }
    case 0xF9: {
        printf("  LDSP, HL\n");
        cpu->SP = cpu->HL;
        break;
    }
    case 0xFA: {
        printf("LD A, (a16)\n");
        uint8_t  l = mmu->read8(cpu->PC + 1);
        uint8_t  h = mmu->read8(cpu->PC + 2);
        uint16_t a = (h << 8) | l;
        cpu->A     = mmu->read8(a);
        cpu->PC    = cpu->PC + 3;
        break;
    }
    case 0xFB: {
        printf("EI\n");
        cpu->_ime = 1;
        cpu->PC += 1;
        break;
    }
    case 0xFC: {
        printf("-- NOT IMPLEMENTED --\n");
        // cpu->PC = cpu->PC + 3;
        break;
    }
    case 0xFD: {
        printf("-- NOT IMPLEMENTED --\n");
        // cpu->PC = cpu->PC + 3;
        break;
    }
    case 0xFE: {
        uint8_t n = mmu->read8(cpu->PC + 1);
        printf("CP A,n8  -- %X --\n", n);
        cpu->set_flag(FLAG_ZERO, n == cpu->A);
        cpu->set_flag(FLAG_SUBTRACT, 1);
        cpu->set_flag(FLAG_HALF_CARRY, (cpu->A & 0x0F) < (n & 0x0F));
        cpu->set_flag(FLAG_CARRY, cpu->A < n);
        cpu->PC = cpu->PC + 2;
        break;
    }
    case 0xFF: {
        printf("RST 38H");
        uint16_t return_addr = cpu->PC + 1;
        push_(return_addr);
        cpu->PC = 0x0038;
        break;
    }
    default: {
        std::cerr << "Unknown opcode: 0x" << opcode << std::hex;
        break;
    }
    }
}

void InstructionSet::pop_(bool condition, uint16_t& reg) {
    if (condition) {
        uint8_t l = mmu->read8(cpu->SP);
        cpu->SP += 1;
        uint8_t h = mmu->read8(cpu->SP);
        cpu->SP += 1;
        reg = (h << 8) | l;
    }
}

void InstructionSet::ret(bool condition) {
    if (condition) {
        uint8_t l = mmu->read8(cpu->SP);
        l += 1;
        uint8_t h = mmu->read8(cpu->SP);
        h += 1;
        cpu->PC = (h << 8) | l;
    }
}

void InstructionSet::or_(uint8_t& reg_1, uint8_t reg_2) {
    uint8_t tmp = reg_1 | reg_2;
    cpu->set_flag(FLAG_ZERO, (tmp == 0));
    cpu->clear_flag(FLAG_SUBTRACT);
    cpu->clear_flag(FLAG_HALF_CARRY);
    cpu->clear_flag(FLAG_CARRY);
    reg_1 = tmp;
}

void InstructionSet::cp_(uint8_t reg_1, uint8_t reg_2) {
    uint16_t tmp = reg_1 - reg_2;
    cpu->set_flag(FLAG_ZERO, tmp == 0);
    cpu->set_flag(FLAG_SUBTRACT, true);
    cpu->set_flag(FLAG_HALF_CARRY, (reg_1 & 0x0F) < (reg_2 & 0x0F));
    cpu->set_flag(FLAG_CARRY, tmp > 0xFF);
}

void InstructionSet::call(bool condition) {
    if (condition) {
        uint8_t  l   = mmu->read8(cpu->PC + 1);
        uint8_t  h   = mmu->read8(cpu->PC + 2);
        uint16_t t   = (h << 8) | l;
        uint16_t ret = cpu->PC += 3;

        cpu->SP -= 1;
        mmu->write8(cpu->SP, ret >> 8);
        cpu->SP -= 1;
        mmu->write8(cpu->SP, ret & 0xFF);
        cpu->PC = t;
    } else {
        cpu->PC += 3;
    }
}

void InstructionSet::and_(uint8_t& reg_1, uint8_t reg_2) {
    reg_1 = reg_1 & reg_2;
    cpu->set_flag(FLAG_ZERO, reg_1 == 0);
    cpu->set_flag(FLAG_HALF_CARRY, 1);
    cpu->clear_flag(FLAG_SUBTRACT);
    cpu->clear_flag(FLAG_CARRY);
}

void InstructionSet::sbc(uint8_t& reg_1, uint8_t reg_2) {
    uint8_t  old    = reg_1;
    uint8_t  a      = reg_1;
    uint8_t  b      = reg_2;
    bool     c      = (cpu->F & FLAG_CARRY);
    uint16_t result = a - b - c;

    reg_1 = result & 0xFF;
    cpu->set_flag(FLAG_ZERO, (reg_1 == 0));
    cpu->set_flag(FLAG_SUBTRACT, true);
    cpu->set_flag(FLAG_HALF_CARRY, (old & 0x0F) < (b & 0x0F) + c);
    cpu->set_flag(FLAG_CARRY, old < (reg_2 + c));
}
void InstructionSet::execute_call() {
    uint16_t address = mmu->read16(cpu->PC + 1);
    cpu->PC += 2;
    cpu->SP -= 2;
    mmu->write16(cpu->SP, cpu->PC);
    cpu->PC = address;
}

void InstructionSet::xor_(uint8_t& reg_1, uint8_t reg_2) {
    reg_1 ^= reg_2;

    cpu->set_flag(FLAG_ZERO, reg_1 == 0);
    cpu->set_flag(FLAG_HALF_CARRY, false);
    cpu->set_flag(FLAG_CARRY, false);
    cpu->set_flag(FLAG_SUBTRACT, false);
}

void InstructionSet::push_(uint16_t reg) {
    cpu->SP -= 1;
    uint8_t h = reg >> 8;
    mmu->write8(cpu->SP, h);
    cpu->SP -= 1;
    uint8_t l = reg & 0xFF;
    mmu->write8(cpu->SP, l);
}

// void InstructionSet::cpl(uint8_t &reg) {
//   // DONE
//   reg = ~reg;
//
//   cp_(uint8_t * reg_1, uint8_t * reg_2) {
//     uint16_t tmp = reg_1 - reg_2;
//     cpu->set_flag(FLAG_ZERO, (reg_1 == reg_2));
//     cpu->set_flag(FLAG_SUBTRACT, 1);
//     cpu->set_flag(FLAG_HALF_CARRY, ((*reg_1 & 0x0F) < (*reg_2 & 0x0F)));
//     cpu->set_flag(FLAG_CARRY, (tmp > 0xFF));
//   }
// }
void InstructionSet::rlc(uint8_t reg) {
    bool msb = reg & 0x80;
    reg      = (reg << 1) | (msb >> 7);

    cpu->set_flag(FLAG_CARRY, msb);

    cpu->set_flag(FLAG_ZERO, reg == 0);

    cpu->clear_flag(FLAG_SUBTRACT);
    cpu->clear_flag(FLAG_HALF_CARRY);
}

void InstructionSet::rrca(uint8_t& reg) {
    // DONE
    bool least_sig_bit = reg & 1;
    reg                = reg >> 1;
    if (least_sig_bit) {
        reg |= 128;
    }
    cpu->set_flag(FLAG_CARRY, least_sig_bit);
    cpu->clear_flag(FLAG_ZERO);
    cpu->clear_flag(FLAG_SUBTRACT);
    cpu->clear_flag(FLAG_HALF_CARRY);
}

// void and_(uint8_t reg_1, uint8_t reg_2) {
//     reg_1 = reg_1 & reg_2;
//     cpu->set_flag(FLAG_ZERO, reg_1 == 0);
//     cpu->clear_flag(FLAG_SUBTRACT);
//     cpu->set_flag(FLAG_HALF_CARRY, 1);
//     cpu->clear_flag(FLAG_CARRY);
// }

void InstructionSet::dec_mem(uint16_t& reg) {
    // To be DONE
    uint8_t tmp          = mmu->read8(reg);
    uint8_t nibble_carry = tmp & 0x0F;
    tmp                  = tmp - 1;
    mmu->write8(reg, tmp);
}

void InstructionSet::adc(uint8_t& reg_1, uint8_t reg_2) {
    bool old_c = cpu->F & FLAG_CARRY;

    uint16_t c = reg_1 + reg_2 + old_c;
    reg_1      = c;
    cpu->set_flag(FLAG_ZERO, reg_1 == 0);
    cpu->clear_flag(FLAG_SUBTRACT);
    cpu->set_flag(FLAG_HALF_CARRY, ((reg_1 & 0x0F) + (reg_2 & 0x0F) + old_c) > 0x0F);
    cpu->set_flag(FLAG_CARRY, c > 0xFF);
}

void InstructionSet::rra() {
    // DONE
    // Before: C = c, A = 76543210
    // After: A = c7654321, C = 0
    bool c  = cpu->F & FLAG_CARRY;
    bool b0 = cpu->A & 1;

    cpu->A = cpu->A >> 1;
    if (c) {
        cpu->A |= 0x80;
    }
    cpu->clear_flag(FLAG_ZERO);
    cpu->clear_flag(FLAG_SUBTRACT);
    cpu->clear_flag(FLAG_HALF_CARRY);
    cpu->set_flag(FLAG_CARRY, b0);
}

void InstructionSet::rla() {
    printf("RLA\n");
    uint8_t old_a     = cpu->A;
    uint8_t old_carry = cpu->F & FLAG_CARRY;
    uint8_t new_carry = (old_a >> 7) & 1;
    cpu->A            = (old_a << 1) | old_carry;

    cpu->set_flag(FLAG_CARRY, new_carry);
    cpu->clear_flag(FLAG_ZERO);
    cpu->clear_flag(FLAG_SUBTRACT);
    cpu->clear_flag(FLAG_HALF_CARRY);
}
void InstructionSet::add8_mem(uint8_t destination, uint8_t value) {
    mmu->write8(destination, destination + value);
    cpu->set_flag(FLAG_ZERO, (destination + value) == 0);
    cpu->clear_flag(FLAG_SUBTRACT);
    cpu->set_flag(FLAG_HALF_CARRY, ((destination & 0x0F) + (value & 0x0F)) > 0x0F);
    cpu->set_flag(FLAG_CARRY, (destination + value) > 0xFF);
}
uint8_t InstructionSet::add8(uint8_t reg_1, uint8_t reg_2) {
    // DONE:
    uint8_t  result = reg_1 + reg_2;
    uint16_t _carry = reg_1 + reg_2;
    cpu->set_flag(FLAG_ZERO, result == 0);
    cpu->set_flag(FLAG_SUBTRACT, false);
    cpu->set_flag(FLAG_HALF_CARRY, (reg_1 & 0x0F) + (reg_2 & 0x0F) >= 0x10);
    cpu->set_flag(FLAG_CARRY, _carry >= 0x0100);
    return result;
}
void InstructionSet::sub(uint8_t& reg_1, uint8_t reg_2) {
    uint8_t a = reg_1;
    uint8_t b = reg_2;

    uint16_t result = a - b;

    reg_1 = result & 0xFF;

    cpu->set_flag(FLAG_ZERO, (reg_1 == 0));
    cpu->set_flag(FLAG_SUBTRACT, true);

    cpu->set_flag(FLAG_HALF_CARRY, (a & 0x0F) < (b & 0x0F));
    cpu->set_flag(FLAG_CARRY, a < b);
}
void InstructionSet::add16(uint16_t& destination, uint16_t& value) {
    // DONE
    uint32_t result = destination + value;
    cpu->clear_flag(FLAG_SUBTRACT);
    cpu->set_flag(FLAG_CARRY, result > 0xFFFF);
    cpu->set_flag(FLAG_HALF_CARRY, ((destination & 0x0FFF) + (value & 0x0FFF)) > 0x0FFF);
    destination = result & 0xFFFF;
}

void InstructionSet::inc(uint8_t& reg) {
    // DONE
    uint8_t old = reg;

    reg = old + 1;

    cpu->set_flag(FLAG_ZERO, reg == 0);
    cpu->clear_flag(FLAG_SUBTRACT);
    cpu->set_flag(FLAG_HALF_CARRY, (old & 0x0F) == 0x0F);
}

void InstructionSet::dec(uint8_t& reg) {
    // DONE

    uint8_t r = reg - 1;
    cpu->set_flag(FLAG_ZERO, r == 0);
    cpu->set_flag(FLAG_SUBTRACT, true);
    cpu->set_flag(FLAG_HALF_CARRY, (reg & 0x0F) == 0x00);
    reg = r;
}

void InstructionSet::inc_mem(uint16_t& reg) {
    // @brief this function increments
    // the value at the [reg],
    // and updates the F flag register
    // accordinly.
    uint8_t tmp          = mmu->read8(reg);
    uint8_t nibble_carry = tmp & 0x0F;
    tmp                  = tmp + 1;
    printf("-- Debug: 0x%X to 0x%X --\n", tmp, reg);
    mmu->write8(reg, tmp);

    cpu->set_flag(FLAG_HALF_CARRY, (nibble_carry == 0x0F));
    cpu->set_flag(FLAG_ZERO, (tmp == 0));
    cpu->clear_flag(FLAG_SUBTRACT);
}

void InstructionSet::cpl(uint8_t& reg) {
    // DONE
    reg = ~reg;

    cpu->set_flag(FLAG_SUBTRACT, true);
    cpu->set_flag(FLAG_HALF_CARRY, true);
}
