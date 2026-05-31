// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
#include "instructions.hpp"
#include "MBC.hpp"
#include "MBC0.hpp"
#include "cpu.hpp"

#include <bitset>
#include <cstdint>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

InstructionSet::InstructionSet(MMU *mmu, Cpu *cpu) {
  this->cpu = cpu;
  this->mmu = mmu;
}

void InstructionSet::ldr(uint16_t &reg) {
  // DONE
  uint8_t tmp_1 = this->mmu->romData[cpu->PC + 1];
  uint8_t tmp_2 = this->mmu->romData[cpu->PC + 2];
  uint16_t tmp = (tmp_2 << 8) | tmp_1;
  reg = tmp;
}

void InstructionSet::ldr(uint8_t &reg) {
  uint8_t tmp = this->mmu->romData[this->cpu->PC + 1];
  reg = tmp;
}

// DONE:
void InstructionSet::ld_mem(uint8_t &reg, uint16_t addr) { mmu->write8(addr, reg); }

void InstructionSet::jump(bool condition, uint8_t offset) {
  // DONE:
  cpu->PC += 2;
  if (condition) {
    cpu->PC += offset;
  }
}
void InstructionSet::inc(uint16_t &reg) {
  uint16_t old = reg;

  reg = old + 1;

  cpu->clear_flag(FLAG_SUBTRACT);
  cpu->set_flag(FLAG_HALF_CARRY, ((old & 0x0FFF) + 1) > 0x0FFF);
}
void InstructionSet::ld(uint16_t &reg, uint16_t addr) { reg = addr; }
void InstructionSet::ld(uint8_t &reg, uint8_t addr) { reg = addr; }

void InstructionSet::execute(uint8_t opcode) {
  //  F flag storage:
  // Bit:  7 6 5 4 3 2 1 0
  //       Z N H C 0 0 0 0
  switch (opcode) {
  case 0x00: {
    // DONE
    // printf("NOP  -- %X --\n", opcode);
    cpu->PC += 1;
    break;
  }
  case 0x01: {
    // DONE
    uint8_t l = mmu->romData[cpu->PC + 1];
    uint8_t h = mmu->romData[cpu->PC + 2];
    uint16_t val = h << 8 | l;
    // printf("LD (BC), A 0x02 -- %X --\n", val);
    cpu->BC = val;
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0x02: {
    // DONE
    ld_mem(cpu->A, cpu->BC);
    // printf("LD (BC), A 0x02 -- %X --\n", cpu->BC);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x03: {
    // DONE
    // printf("INC BC 0x03 -- before %X --\n", cpu->BC);
    cpu->BC = cpu->BC + 1;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x04: {
    // DONE
    // printf("INC B 0x04 -- before %X --\n", cpu->B);
    inc(cpu->B);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x05: {
    // DONE
    // printf("DEC B 0x05 -- before %X --\n", cpu->B);
    dec(cpu->B);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x06: {
    // DONE:
    // printf("LD B, d8 0x06 --- before %X --\n", cpu->B);
    cpu->B = mmu->romData[cpu->PC + 1];
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x07: {
    // DONE
    uint16_t result = cpu->A << 1;
    bool b = (cpu->A << 8) & 1;
    // printf("RLCA -- %d --\n", b);
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
    // DONE. but check:
    uint8_t l = mmu->romData[cpu->PC + 1];
    uint8_t h = mmu->romData[cpu->PC + 2];
    uint16_t address = (h << 8) | l;
    // printf("LD (a16), SP -- %X --\n", address);
    uint8_t value = cpu->SP & 0xFF;
    mmu->write8(address, value);
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0x09: {
    // DONE. but check:
    cpu->HL = cpu->HL + cpu->BC;
    uint32_t tmp = cpu->HL + cpu->BC;
    // printf("ADD HL, BC -- %X --\n", cpu->HL);
    cpu->set_flag(FLAG_ZERO, 0);
    cpu->set_flag(FLAG_CARRY, tmp > 0xFFFF);
    cpu->set_flag(FLAG_SUBTRACT, ((cpu->HL & 0x0FFF) + (cpu->BC & 0x0FFF)) > 0x0FFF);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x0A: {
    // DONE
    // printf("LD A, (BC)\n");
    cpu->A = mmu->read8(cpu->BC);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x0B: {
    // DONE
    cpu->BC = cpu->BC - 1;
    // printf("DEC BC -- %X --\n", cpu->BC);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x0C: {
    // DONE
    inc(cpu->C);
    // printf("INC C -- %X --\n", cpu->C);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x0D: {
    // DONE
    dec(cpu->C);
    // printf("DEC C -- %X --", cpu->C);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x0E: {
    // DONE
    cpu->C = mmu->romData[cpu->PC + 1];
    // printf("LD C, d8 -- %X --\n", cpu->C);
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x0F: {
    // DONE
    rrca(cpu->A);
    // printf("RRCA --%X --", cpu->A);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x10: {
    // this one should be checked out
    // printf("STOP 0x10 \n");
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x11: {
    // DONE
    ldr(cpu->DE);
    // printf("LD (DE), d16 -- %X --\n", cpu->DE);
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0x12: {
    // DONE
    // printf("LD (DE), A -- %X --\n", cpu->DE);
    mmu->write8(cpu->DE, cpu->A);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x13: {
    // DONE
    cpu->DE = cpu->DE + 1;
    // printf("INC DE -- %X --\n", cpu->DE);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x14: {
    // DONE
    inc(cpu->D);
    // printf("INC D, -- %X --\n", cpu->D);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x15: {
    // DONE
    dec(cpu->D);
    // printf("DEC D -- %X --\n", cpu->D);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x16: {
    // DONE
    cpu->D = mmu->romData[cpu->PC + 1];
    // printf("LD D, d8 -- %X --\n", cpu->D);
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x17: {
    // DONE
    // printf("RLA\n");
    rla();
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x18: {
    // DONE
    int8_t offset;
    offset = static_cast<int8_t>(mmu->romData[cpu->PC + 1]);
    cpu->PC += offset;
    // printf("JR r8 -- %X --\n", offset);
    cpu->PC += 2;
    break;
  }
  case 0x19: {
    // DONE
    add16(cpu->HL, cpu->DE);
    cpu->PC = cpu->PC + 1;
    // printf("ADD HL, DE --%X --\n", cpu->HL);
    break;
  }
  case 0x1A: {
    // DONE
    cpu->A = mmu->read8(cpu->DE);
    // printf("LD A, (DE) -- %X --\n", cpu->A);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x1B: {
    // DONE
    cpu->DE = cpu->DE - 1;
    // printf("DEC DE -- %X --\n", cpu->DE);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x1C: {
    // DONE
    inc(cpu->E);
    // printf("INC E -- %X --\n", cpu->E);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x1D: {
    // DONE
    dec(cpu->E);
    // printf("DEC E -- %X --\n", cpu->E);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x1E: {
    // printf("LD E, d8");
    cpu->E = mmu->romData[cpu->PC + 1];
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x1F: {
    // TODO: 0x1F
    // printf("RRA\n");
    rra();
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x20: {
    // DONE
    // printf("JR NZ, s8");
    jump(!(cpu->F & FLAG_ZERO), mmu->read8(cpu->PC + 1));
    break;
  }
  case 0x21: {
    // DONE:
    uint8_t l = mmu->read8(cpu->PC + 1);
    uint8_t h = mmu->read8(cpu->PC + 2);

    uint16_t v = l | (h << 8);
    ld(cpu->HL, v);
    // printf("LD HL, d16 0x21 -- (HL == %X) --\n", cpu->HL);
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0x22: {
    // DONE
    // print("LD (HL+), A");
    ld_mem(cpu->A, cpu->HL);
    cpu->HL += 1;
    cpu->PC += 1;
    break;
  }
  case 0x23: {
    // DONE:
    // printf("INC HL 0x23\n");
    cpu->HL = cpu->HL + 1;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x24: {
    // DONE
    // printf"INC H\n");
    inc(cpu->HL);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x25: {
    // soon
    // printf("DEC H");
    dec(cpu->H);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x26: {
    // printf("LD H, d8");
    ld(cpu->H, mmu->read8(cpu->PC + 1));
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x27: {
    // DONE
    // printf("DAA");
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
    // DONE:
    // printf("JR Z, r8 0x28");
    int8_t offset = mmu->read8(cpu->PC + 1);
    jump(cpu->F & FLAG_ZERO, offset);
    // pc shouldn't not be updated here
    break;
  }
  case 0x29: {
    // DONE
    // std::cout << "ADD HL, HL" << std::endl;
    add16(cpu->HL, cpu->HL);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x2A: {
    // DONE
    // printf("LD A, (HL+)\n");
    ld_mem(cpu->A, cpu->HL);
    cpu->HL += 1;
    cpu->PC += 1;
    break;
  }
  case 0x2B: {
    // DONE
    // printf("DEC HL\n");
    cpu->HL -= 1;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x2C: {
    // DONE
    // printf("INC L\n");
    inc(cpu->L);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x2D: {
    // DONE
    // printf("DEC L\n");
    dec(cpu->L);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x2E: {
    // DONE
    // printf("LD L, d8\n");
    ld_mem(cpu->L, mmu->read8(cpu->PC + 1));
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x2F: {
    // printf("CPL\n");
    cpl(cpu->A);
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x30: { // JR NC,r8
    // DONE:
    // printf("JR NC,r8 -- %d --\n");
    jump(!(cpu->F & FLAG_CARRY), mmu->read8(cpu->PC + 1));
    break;
  }
  case 0x31: {
    // DONE:
    // printf("LD SP, d16\n");
    ldr(cpu->SP);
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0x32: {
    // DONE:
    // printf("LD (HL-), A\n");
    ld_mem(cpu->A, cpu->HL);
    cpu->HL -= 1;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x33: {
    // DONE:
    // printf("INC SP\n");
    cpu->SP = cpu->SP + 1;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x34: {
    // DONE:
    // printf("INC (HL) 0x34\n");
    inc_mem(cpu->HL);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x35: {
    // DONE:
    dec_mem(cpu->HL);
    // printf("DEC (HL) 0x35 0x%X \n", mmu->read8(cpu->HL));
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x36: {
    // DONE:
    // printf("LD (HL), d8\n");
    mmu->write8(cpu->HL, mmu->read8(cpu->PC + 1));
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x37: {
    // DONE:
    // printf("SCF\n");
    cpu->set_flag(FLAG_CARRY, true);
    cpu->clear_flag(FLAG_SUBTRACT);
    cpu->clear_flag(FLAG_HALF_CARRY);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x38: {
    // DONE:
    // printf("JR C, r8 0x38\n");
    jump(cpu->F & FLAG_CARRY, mmu->read8(cpu->PC + 1));
    // don't change pc on jumps
    break;
  }
  case 0x39: {
    // DONE
    // printf("ADD HL, SP\n");
    add16(cpu->HL, cpu->SP);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x3A: {
    // DONE
    // printf("LD A, (HL-)\n");
    cpu->A = mmu->read8(cpu->HL);
    cpu->HL = cpu->HL - 1;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x3B: {
    // DONE
    // printf("DEC SP\n");
    cpu->SP = cpu->SP - 1;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x3C: {
    // DONE
    // printf("INC A\n");
    inc(cpu->A);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x3D: {
    // DONE
    // printf("DEC A TODO\n");
    dec(cpu->A);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x3E: {
    // DONE:
    cpu->A = mmu->read8(cpu->PC + 1);
    // printf("LD A, d8: 0x3E A: -- %X -- \n");
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x3F: {
    // DONE:
    // printf("CCF\n");
    bool carry = cpu->F & FLAG_CARRY;
    cpu->set_flag(FLAG_CARRY, !carry);
    cpu->set_flag(FLAG_SUBTRACT, false);
    cpu->set_flag(FLAG_HALF_CARRY, false);
    break;
  }
  case 0x40: {
    // DONE:
    // printf("CCFLD B, B\n");
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x41: {
    // DONE
    // printf("LD B, C\n");
    cpu->B = cpu->C;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x42: {
    // DONE
    // printf("LD B, D\n");
    cpu->B = cpu->D;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x43: {
    // printf("LD B, E\n";
    cpu->B = cpu->E;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x44: {
    // printf("LD B, H\n");
    cpu->B = cpu->H;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x45: {
    // printf("LD B, L\n");
    cpu->B = cpu->L;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x46: {
    // DONE
    // printf("LD B, (HL)");
    cpu->B = mmu->read16(cpu->HL);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x47: {
    // DONE
    printf("LD B, A\n");
    cpu->B = cpu->A;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x48: {
    // DONE
    printf("LD C, B\n");
    cpu->C = cpu->B;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x49: {
    // DONE
    printf("LD C, C\n");
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x4A: {
    // DONE
    printf("LD C, D\n");
    cpu->C = cpu->C;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x4B: {
    // DONE
    printf("LD C, E\n");
    cpu->C = cpu->E;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x4C: {
    // DONE
    printf("LD C, H\n");
    cpu->C = cpu->H;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x4D: {
    // DONE
    printf("LD C, L\n");
    cpu->C = cpu->L;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x4E: {
    // DONE
    printf("LD C, (HL)\n");
    cpu->C = mmu->read8(cpu->HL);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x4F: {
    // DONE
    printf("LD C, A\n");
    cpu->C = cpu->A;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x50: {
    // DONE
    printf("LD B, D\n");
    cpu->B = cpu->D;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x51: {
    // DONE
    printf("LD D, C\n");
    cpu->B = cpu->C;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x52: {
    // DONE
    printf("LD C, A\n");
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x53: {
    // DONE
    printf("LD D, E\n");
    cpu->D = cpu->E;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x54: {
    // DONE
    printf("LD D, H\n");
    cpu->D = cpu->H;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x55: {
    // DONE
    printf("LD D, L\n");
    cpu->D = cpu->L;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x56: {
    // DONE
    printf("LD D, (HL)\n");

    cpu->D = mmu->read8(cpu->HL);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x57: {
    // DONE
    printf("LD D, A\n");
    cpu->D = cpu->A;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x58: {
    // DONE
    printf("LD D, B\n");
    cpu->D = cpu->B;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x59: {
    // DONE
    printf("LD E, C\n");
    cpu->E = cpu->C;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x5A: {
    // DONE
    printf("LD E, D\n");
    cpu->E = cpu->D;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x5B: {
    // DONE
    printf("LD D, B\n");
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x5C: {
    // DONE
    printf("LD E, H\n");
    cpu->E = cpu->H;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x5D: {
    // DONE
    printf("LD E, L\n");
    cpu->E = cpu->L;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x5E: {
    // DONE
    printf("LD E, L\n");
    cpu->E = mmu->read8(cpu->HL);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x5F: {
    // DONE
    printf("LD E, A\n");
    cpu->E = cpu->A;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x60: {
    // DONE
    printf("LD H, B\n");
    cpu->H = cpu->B;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x61: {
    // DONE
    printf("LD H, C\n");
    cpu->H = cpu->C;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x62: {
    // DONE
    printf("LD H, D\n");
    cpu->H = cpu->D;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x63: {
    // DONE
    printf("LD H, E\n");
    cpu->H = cpu->E;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x64: {
    // DONE
    printf("LD H, H\n");
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x65: {
    // DONE
    printf("LD H, L\n");
    cpu->H = cpu->L;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x66: {
    // DONE
    printf("LD H, (HL)\n");
    cpu->H = mmu->read8(cpu->HL);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x67: {
    // DONE
    printf("LD H, A\n");
    cpu->H = cpu->A;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x68: {
    // DONE
    printf("LD L, B\n");
    cpu->L = cpu->B;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x69: {
    // DONE
    printf("LD L, C\n");
    cpu->L = cpu->C;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x6A: {
    // DONE
    printf("LD L, D\n");
    cpu->L = cpu->D;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x6B: {
    // DONE
    printf("LD L, E\n");
    cpu->L = cpu->E;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x6C: {
    // DONE
    printf("LD L, H\n");
    cpu->L = cpu->H;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x6D: {
    // DONE
    printf("LD L, L\n");
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x6E: {
    // DONE
    printf("LD L, (LH)\n");
    cpu->L = mmu->read8(cpu->HL);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x6F: {
    // DONE
    printf("LD L, A\n");
    cpu->L = cpu->A;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x70: {
    // DONE
    printf("LD (HL), B\n");
    mmu->write8(cpu->HL, cpu->B);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x71: {
    // DONE
    printf("LD (HL), C\n");
    mmu->write8(cpu->HL, cpu->C);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x72: {
    // DONE
    printf("LD (HL), D\n");
    mmu->write8(cpu->HL, cpu->D);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x73: {
    // DONE
    printf("LD (HL), E\n");
    mmu->write8(cpu->HL, cpu->E);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x74: {
    // DONE
    printf("LD (HL), H\n");
    mmu->write8(cpu->HL, cpu->H);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x75: {
    // DONE
    printf("LD (HL), L\n");
    mmu->write8(cpu->HL, cpu->L);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x76: {
    // DONE
    cpu->halted = true;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x77: {
    // DONE
    printf("LD (HL), A\n");
    mmu->write8(cpu->HL, cpu->A);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x78: {
    // DONE
    printf("LD A, B\n");
    cpu->A = cpu->B;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x79: {
    // DONE
    printf("LD A, C\n");
    cpu->A = cpu->C;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x7A: {
    // DONE
    printf("LD A, D\n");
    cpu->A = cpu->D;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x7B: {
    // DONE
    printf("LD A, E\n");
    cpu->A = cpu->E;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x7C: {
    // DONE
    printf("LD A, H\n");
    cpu->A = cpu->H;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x7D: {
    // DONE
    printf("LD A, L\n");
    cpu->A = cpu->L;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x7E: {
    // DONE
    printf("LD A, (HL)\n");
    cpu->A = mmu->read8(cpu->HL);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x7F: {
    // DONE
    printf("LD A, A\n");
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x80: {
    // DONE
    printf("ADD A, B\n");
    add8(cpu->A, cpu->B);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x81: {
    // DONE
    printf("ADD A, C\n");
    add8(cpu->A, cpu->C);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x82: {
    // DONE
    printf("ADD A, D\n");
    add8(cpu->A, cpu->D);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x83: {
    // DONE
    printf("ADD A, E\n");
    add8(cpu->A, cpu->E);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x84: {
    // DONE
    // printf("ADD A, H\n");
    add8(cpu->A, cpu->H);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x85: {
    // DONE
    // printf("ADD A, L\n");
    add8(cpu->A, cpu->L);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x86: {
    // DONE
    // printf("ADD A, (HL)\n");
    add8_mem(cpu->A, mmu->romData[cpu->HL]);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x87: {
    // DONE
    // printf("ADD A, A\n");
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x88: {
    // DONE
    // printf("ADD A, B\n");
    adc(cpu->A, cpu->B);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x89: {
    // DONE
    // printf("ADC A, C\n");
    adc(cpu->A, cpu->C);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x8A: {
    // DONE
    // printf("ADC A, D\n");
    adc(cpu->A, cpu->D);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x8B: {
    // DONE
    // printf("ADC A, E\n");
    adc(cpu->A, cpu->E);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x8C: {
    // DONE
    printf("ADC A, H\n");
    adc(cpu->A, cpu->H);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x8D: {
    // DONE
    printf("ADC A, L\n");
    adc(cpu->A, cpu->L);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x8E: {
    // DONE
    // printf("ADC A, (HL)\n");
    adc(cpu->A, mmu->read8(cpu->L));
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x8F: {
    // DONE
    // printf("ADC A, L\n");
    adc(cpu->A, cpu->A);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x90: {
    // DONE
    // printf("SUB A, L\n");
    sub(cpu->A, cpu->B);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x91: {
    // DONE
    // printf("SUB C\n");
    sub(cpu->A, cpu->C);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x92: {
    // DONE
    // printf("SUB D\n");
    sub(cpu->A, cpu->D);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x93: {
    // DONE
    // printf("SUB E\n");
    sub(cpu->A, cpu->E);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x94: {
    // DONE
    // printf("SUB H\n");
    sub(cpu->A, cpu->H);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x95: {
    // DONE
    // printf("SUB L\n");
    sub(cpu->A, cpu->L);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x96: {
    // DONE
    // printf("SUB (HL)\n");
    sub(cpu->A, cpu->HL);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x97: {
    // DONE
    // printf("SUB A\n");
    sub(cpu->A, cpu->A);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x98: {
    // DONE
    // printf("SBC A, B\n");
    sbc(cpu->A, cpu->B);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x99: {
    // DONE
    // printf("SBC A, C\n");
    sbc(cpu->A, cpu->C);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x9A: {
    // DONE
    // printf("SBC A, D\n");
    sbc(cpu->A, cpu->D);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x9B: {
    // DONE
    // printf("SBC A, E\n");
    sbc(cpu->A, cpu->E);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x9C: {
    // DONE
    // printf("SBC A, H\n");
    sbc(cpu->A, cpu->H);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x9D: {
    // DONE
    // printf("SBC A, L\n");
    sbc(cpu->A, cpu->L);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x9E: {
    // DONE
    // printf("SBC A, L\n");
    sbc(cpu->A, mmu->read8(cpu->HL));
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x9F: {
    // DONE
    // printf("SBC A, A\n");
    sbc(cpu->A, cpu->A);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xA0: {
    // DONE
    // printf("AND A, B\n");
    and_(cpu->A, cpu->B);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xA1: {
    // DONE
    // printf("AND A, C\n");
    and_(cpu->A, cpu->C);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xA2: {
    // DONE
    // printf("AND A, C\n");
    and_(cpu->A, cpu->D);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xA3: {
    // DONE
    // printf("AND A, E\n");
    and_(cpu->A, cpu->E);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xA4: {
    // DONE
    // printf("AND A, H\n");
    and_(cpu->A, cpu->H);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xA5: {
    // DONE
    // printf("AND A, L\n");
    and_(cpu->A, cpu->L);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xA6: {
    // DONE
    // printf("AND A, (HL)\n");
    and_(cpu->A, mmu->read8(cpu->L));
    cpu->PC = cpu->PC + 1;
    break;
  }

  case 0xA7: {
    // DONE
    // printf("AND A, (HL)\n");
    and_(cpu->A, mmu->read8(cpu->L));
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xA8: {
    // DONE
    // printf("XOR A, B\n");
    xor_(cpu->A, cpu->B);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xA9: {
    // DONE
    // printf("XOR A, C\n");
    xor_(cpu->A, cpu->C);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xAA: {
    // DONE
    // printf("XOR A, D\n");
    xor_(cpu->A, cpu->D);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xAB: {
    // DONE
    // printf("XOR A, E\n");
    xor_(cpu->A, cpu->E);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xAC: {
    // DONE
    // printf("XOR A, H\n");
    xor_(cpu->A, cpu->H);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xAD: {
    // DONE
    // printf("XOR A, L\n");
    xor_(cpu->A, cpu->L);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xAE: {
    // DONE
    // printf("XOR A, (HL)\n");
    xor_(cpu->A, mmu->read8(cpu->HL));
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xAF: {
    // DONE
    // printf("XOR A, A\n");
    xor_(cpu->A, cpu->A);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xB0: {
    // DONE
    // printf("OR A, B\n");
    or_(cpu->A, cpu->B);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xB1: {
    // DONE
    // printf("OR A, C\n");
    or_(cpu->A, cpu->C);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xB2: {
    // DONE
    // printf("OR A, D\n");
    or_(cpu->A, cpu->D);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xB3: {
    // DONE
    // printf("OR A, E\n");
    or_(cpu->A, cpu->E);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xB4: {
    // DONE
    // printf("OR A, H\n");
    or_(cpu->A, cpu->H);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xB5: {
    // DONE
    // printf("OR A, L\n");
    or_(cpu->A, cpu->L);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xB6: {
    // DONE
    // printf("OR A, (HL)\n");
    or_(cpu->A, mmu->read8(cpu->HL));
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xB7: {
    // DONE
    // printf("OR A, A\n");
    or_(cpu->A, cpu->A);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xB8: {
    // DONE
    // printf("CP B\n");
    cp_(cpu->A, cpu->B);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xB9: {
    // DONE
    // printf("CP C\n");
    cp_(cpu->A, cpu->C);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xBA: {
    // DONE
    // printf("CP D\n");
    cp_(cpu->A, cpu->D);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xBB: {
    // DONE
    // printf("CP E\n");
    cp_(cpu->A, cpu->E);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xBC: {
    // DONE
    // printf("CP H\n");
    cp_(cpu->A, cpu->H);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xBD: {
    // DONE
    // printf("CP L\n");
    cp_(cpu->A, cpu->L);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xBE: {
    // DONE
    // printf("CP (HL)\n");
    cp_(cpu->A, mmu->read8(cpu->HL));
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xBF: {
    // DONE
    // printf("CP A\n");
    cp_(cpu->A, cpu->A);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xC0: {
    // DONE
    // printf("RET NZ\n");
    if (!(cpu->F & FLAG_ZERO)) {
      ret(true);
    } else {
      cpu->PC = cpu->PC + 1;
    }
    break;
  }
  case 0xC1: {
    // DONE
    // printf("POP BC\n");
    cpu->C = mmu->read8(cpu->SP);
    cpu->B = mmu->read8(cpu->SP + 1);
    cpu->SP += 1;
    break;
  }
  case 0xC2: {
    // DONE
    // print("JP NZ, nn\n");
    if (!(cpu->F & FLAG_ZERO)) {
      cpu->PC = mmu->read8(cpu->PC) | mmu->read8(cpu->PC + 1) << 8;
    } else {
      cpu->PC += 3;
    }
    break;
  }
  case 0xC3: {
    // DONE
    // printf("JP a16\n");
    uint8_t l = mmu->read8(cpu->PC + 1);
    uint8_t h = mmu->read8(cpu->PC + 2);
    cpu->PC = (h << 8) | l;
    break;
  }
  case 0xC4: {
    // DONE
    // printf("CALL nn 0xc4");
    call(!(cpu->F & FLAG_ZERO));
    // no reason to increment program counter
    break;
  }
  case 0xC5: {
    // DONE
    // printf("PUSH BC\n");
    push_(cpu->BC);
    cpu->PC += 1;
    break;
  }
  case 0xC6: {
    // DONE
    // printf("ADD A, d8\n");
    add8(cpu->A, mmu->read8(cpu->PC + 1));
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xC7: {
    // DONE
    // printf("RST 00h\n");
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
    // DONE
    // printf("RET Z\n");
    if (cpu->F & FLAG_ZERO) {
      ret(true);
    } else {
      cpu->PC += 1;
    }
    break;
  }
  case 0xC9: {
    // DONE
    // printf("RET\n");
    pop_(true, cpu->PC);
    break;
  }
  case 0xCA: {
    // DONE
    // printf("JP Z, nn 0xCA\n");
    if (cpu->F & FLAG_ZERO) {
      u_int8_t l = mmu->read8(cpu->PC + 1);
      u_int8_t h = mmu->read8(cpu->PC + 2);
      cpu->PC = (h << 8) | l;
    } else {
      cpu->PC += 3;
    }
    break;
  }
  case 0xCB: {
    // printf("--- PREFIX CB NOTDONE 0xCB -- \n");
    switch (mmu->read8(cpu->PC + 1)) {
    case 0x00: {
      // DONE
      // printf("RLC B\n");
      rlc_extended(cpu->B);
      cpu->PC += 2;
      break;
    }
    case 0x01: {
      // DONE
      // printf("RLC c\n");
      rlc_extended(cpu->C);
      cpu->PC += 2;
      break;
    }
    case 0x02: {
      // DONE
      //  printf("rlc d\n");
      rlc_extended(cpu->D);
      cpu->PC += 2;
      break;
    }
    case 0x03: {
      // DONE
      // printf("RLC E\n");
      rlc_extended(cpu->E);
      cpu->PC += 2;
      break;
    }
    case 0x04: {
      // DONE
      // printf("RLC H\n");
      rlc_extended(cpu->H);
      cpu->PC += 2;
      break;
    }
    case 0x05: {
      // DONE
      // printf("RLC L\n");
      rlc_extended(cpu->L);
      cpu->PC += 2;
      break;
    }
    case 0x06: {
      // DONE
      // printf("RLC (HL)\n");
      uint8_t v = mmu->read8(cpu->HL);
      rlc_extended(v);
      mmu->write8(cpu->HL, v);
      cpu->PC += 2;
      break;
    }
    case 0x07: {
      // DONE
      // printf("RLC A\n");
      rlc_extended(cpu->A);
      cpu->PC += 2;
      break;
    }
    case 0x08: {
      // DONE
      // printf("RRC B\n");
      rrc_extended(cpu->B);
      cpu->PC += 2;
      break;
    }
    case 0x09: {
      // DONE
      // printf("RRC C\n");
      rrc_extended(cpu->C);
      cpu->PC += 2;
      break;
    }
    case 0x0A: {
      // DONE
      // printf("RRC D\n");
      rrc_extended(cpu->D);
      cpu->PC += 2;
      break;
    }
    case 0x0B: {
      // DONE
      // printf("RRC E\n");
      rrc_extended(cpu->E);
      cpu->PC += 2;
      break;
    }
    case 0x0C: {
      // DONE
      // printf("RRC H\n");
      rrc_extended(cpu->H);
      cpu->PC += 2;
      break;
    }
    case 0x0D: {
      // DONE
      // printf("RRC L\n");
      rrc_extended(cpu->L);
      cpu->PC += 2;
      break;
    }
    case 0x0E: {
      // DONE
      // printf("RRC (HL)\n");
      uint8_t v = mmu->read8(cpu->HL);
      rrc_extended(v);
      mmu->write8(cpu->HL, v);
      cpu->PC += 2;
      break;
    }
    case 0x0F: {
      // DONE
      // printf("RL B\n");
      rrc_extended(cpu->A);
      cpu->PC += 2;
      break;
    }
    case 0x10: {
      // DONE
      // printf("RL B \n");
      rl_extended(cpu->B);
      cpu->PC += 2;
      break;
    }
    case 0x11: {
      // DONE
      // printf("RL C\n");
      rl_extended(cpu->C);
      cpu->PC += 2;
      break;
    }
    case 0x12: {
      // DONE
      // printf"RL D\n");
      rl_extended(cpu->D);
      cpu->PC += 2;
      break;
    }
    case 0x13: {
      // DONE
      // printf("RL E\n");
      rl_extended(cpu->E);
      cpu->PC += 2;
      break;
    }
    case 0x14: {
      // DONE
      // printf("RL H\n");
      rl_extended(cpu->H);
      cpu->PC += 2;
      break;
    }
    case 0x15: {
      // DONE
      // printf("RL L \n");
      rl_extended(cpu->L);
      cpu->PC += 2;
      break;
    }
    case 0x16: {
      // DONE
      // printf("RL (HL)");
      uint8_t v = mmu->read8(cpu->HL);
      rl_extended(v);
      mmu->write8(cpu->HL, v);
      cpu->PC += 2;
      break;
    }
    case 0x17: {
      // DONE
      // printf("RL A\n");
      rl_extended(cpu->A);
      cpu->PC += 2;
      break;
    }
    case 0x18: {
      // DONE
      // printf("RR B\n");
      rr_extended(cpu->B);
      cpu->PC += 2;
      break;
    }
    case 0x19: {
      // DONE
      // printf("RR B\n");
      rr_extended(cpu->C);
      cpu->PC += 2;
      break;
    }
    case 0x1A: {
      // DONE
      // printf("RR D\n");
      rr_extended(cpu->D);
      cpu->PC += 2;
      break;
    }
    case 0x1B: {
      // DONE
      // printf("RR D\n");
      rr_extended(cpu->E);
      cpu->PC += 2;
      break;
    }
    case 0x1C: {
      // DONE
      // printf("RR H\n");
      rr_extended(cpu->H);
      cpu->PC += 2;
      break;
    }
    case 0x1D: {
      // done
      // printf("RR L\n");
      rr_extended(cpu->L);
      cpu->PC += 2;
      break;
    }
    case 0x1E: {
      // done
      // printf("RR L\n");
      uint8_t v = mmu->read8(cpu->HL);
      rr_extended(v);
      mmu->write8(cpu->HL, v);
      cpu->PC += 2;
      break;
    }
    case 0x1F: {
      // done
      // printf("RR A\n");
      rr_extended(cpu->A);
      cpu->PC += 2;
      break;
    }
    case 0x20: {
      // DONE
      // printf("SLA B\n");
      sla_extended(cpu->B);
      cpu->PC += 2;
      break;
    }
    case 0x21: {
      // DONE
      // printf("SLA C\n");
      sla_extended(cpu->C);
      cpu->PC += 2;
      break;
    }
    case 0x22: {
      // DONE
      // printf("SLA D\n");
      sla_extended(cpu->D);
      cpu->PC += 2;
      break;
    }
    case 0x23: {
      // DONE
      // printf("SLA E\n");
      sla_extended(cpu->E);
      cpu->PC += 2;
      break;
    }
    case 0x24: {
      // DONE
      // printf("SLA H\n");
      sla_extended(cpu->H);
      cpu->PC += 2;
      break;
    }
    case 0x25: {
      // DONE
      // printf("SLA L\n");
      sla_extended(cpu->L);
      cpu->PC += 2;
      break;
    }
    case 0x26: {
      // DONE
      // printf("SLA (HL)\n");
      uint8_t v = mmu->read8(cpu->HL);
      sla_extended(v);
      mmu->write8(cpu->HL, v);
      cpu->PC += 2;
      break;
    }
    case 0x27: {
      // DONE
      // printf("SLA A\n");
      sla_extended(cpu->A);
      cpu->PC += 2;
      break;
    }
    case 0x28: {
      // DONE
      // printf("SLA B\n");
      sra_extended(cpu->B);
      cpu->PC += 2;
      break;
    }
    case 0x29: {
      // TODO
      // printf("SRA C\n");
      sra_extended(cpu->C);
      cpu->PC += 2;
      break;
    }
    case 0x2A: {
      // TODO
      // printf("SRA C\n");
      sra_extended(cpu->D);
      cpu->PC += 2;
      break;
    }
    case 0x2B: {
      // TODO
      // printf("SRA D\n");
      sra_extended(cpu->E);
      cpu->PC += 2;
      break;
    }
    case 0x2C: {
      // done
      // printf("SRA E\n");
      sra_extended(cpu->H);
      cpu->PC += 2;
      break;
    }
    case 0x2D: {
      // DONE
      // printf("SRA L\n");
      sra_extended(cpu->L);
      cpu->PC += 2;
      break;
    }
    case 0x2E: {
      // DONE
      // printf("SRA (HL)\n");
      uint8_t v = mmu->read8(cpu->HL);
      sra_extended(v);
      mmu->write8(cpu->HL, v);
      cpu->PC += 2;
      break;
    }
    case 0x2F: {
      // DONE
      // printf("SRA A\n");
      sra_extended(cpu->A);
      cpu->PC += 2;
      break;
    }
    case 0x30: {
      // DONE
      // printf("SWAP B\n");
      swap_extended(cpu->B);
      cpu->PC += 2;
      break;
    }
    case 0x31: {
      // DONE
      // printf("SWAP C\n");
      swap_extended(cpu->C);
      cpu->PC += 2;
      break;
    }
    case 0x32: {
      // DONE
      // printf("SWAP D\n");
      swap_extended(cpu->D);
      cpu->PC += 2;
      break;
    }
    case 0x33: {
      // DONE
      // printf("SWAP E\n");
      swap_extended(cpu->E);
      cpu->PC += 2;
      break;
    }
    case 0x34: {
      // DONE
      // printf("SWAP H\n");
      swap_extended(cpu->H);
      cpu->PC += 2;
      break;
    }
    case 0x35: {
      // DONE
      // printf("SWAP L\n");
      swap_extended(cpu->L);
      cpu->PC += 2;
      break;
    }
    case 0x36: {
      // DONE
      // printf("SWAP (HL)\n");
      uint8_t v = mmu->read8(cpu->HL);
      swap_extended(v);
      mmu->write8(cpu->HL, v);
      cpu->PC += 2;
      break;
    }
    case 0x37: {
      // DONE
      // printf("SWAP A\n");
      swap_extended(cpu->A);
      cpu->PC += 2;
      break;
    }
    case 0x38: {
      // DONE
      // printf("SRL B\n");
      srl_extended(cpu->B);
      cpu->PC += 2;
      break;
    }
    case 0x39: {
      // DONE
      printf("SRL C\n");
      srl_extended(cpu->C);
      cpu->PC += 2;
      break;
    }
    case 0x3A: {
      // DONE
      // printf("SRL D\n");
      srl_extended(cpu->D);
      cpu->PC += 2;
      break;
    }
    case 0x3B: {
      // DONE
      // printf("SRL E\n");
      srl_extended(cpu->E);
      cpu->PC += 2;
      break;
    }
    case 0x3C: {
      // DONE
      // printf("SRL H\n");
      srl_extended(cpu->H);
      cpu->PC += 2;
      break;
    }
    case 0x3D: {
      // DONE
      // printf("SRL L\n");
      srl_extended(cpu->L);
      cpu->PC += 2;
      break;
    }
    case 0x3E: {
      // DONE
      // printf("SRL (HL)\n");
      uint8_t v = mmu->read8(cpu->HL);
      srl_extended(v);
      mmu->write8(cpu->HL, v);
      cpu->PC += 2;
      break;
    }
    case 0x3F: {
      // DONE
      // printf("SRL A\n");
      srl_extended(cpu->A);
      cpu->PC += 2;
      break;
    }
    case 0x40: {
      // DONE
      // printf("BIT0 B\n");
      bit0_extended(cpu->B);
      cpu->PC += 2;
      break;
    }
    case 0x41: {
      // DONE
      // printf("BIT0 C\n");
      bit0_extended(cpu->C);
      cpu->PC += 2;
      break;
    }
    case 0x42: {
      // DONE
      // printf("BIT0 D\n");
      bit0_extended(cpu->D);
      cpu->PC += 2;
      break;
    }
    case 0x43: {
      // DONE
      // printf("BIT0 E\n");
      bit0_extended(cpu->E);
      cpu->PC += 2;
      break;
    }
    case 0x44: {
      // DONE
      // printf("BIT0 H\n");
      bit0_extended(cpu->H);
      cpu->PC += 2;
      break;
    }
    case 0x45: {
      // DONE
      // printf("BIT0 L\n");
      bit0_extended(cpu->L);
      cpu->PC += 2;
      break;
    }
    case 0x46: {
      // DONE
      // printf("BIT0 (HL)\n");
      uint8_t v = mmu->read8(cpu->HL);
      bit0_extended(v);
      mmu->write8(cpu->HL, v);
      cpu->PC += 2;
      break;
    }
    case 0x47: {
      // DONE
      // printf("BIT0 A\n");
      bit0_extended(cpu->A);
      cpu->PC += 2;
      break;
    }
    case 0x48: {
      // DONE
      // printf("BIT1 B\n");
      bit1_extended(cpu->B);
      cpu->PC += 2;
      break;
    }
    case 0x49: {
      // DONE
      // printf("BIT1 C\n");
      bit1_extended(cpu->C);
      cpu->PC += 2;
      break;
    }
    case 0x4A: {
      // DONE
      // printf("BIT1 D\n");
      bit1_extended(cpu->D);
      cpu->PC += 2;
      break;
    }
    case 0x4B: {
      // DONE
      // printf("BIT1 E\n");
      bit1_extended(cpu->E);
      cpu->PC += 2;
      break;
    }
    case 0x4C: {
      // DONE
      // printf("BIT1 H\n");
      bit1_extended(cpu->H);
      cpu->PC += 2;
      break;
    }
    case 0x4D: {
      // DONE
      // printf("BIT1 L\n");
      bit1_extended(cpu->L);
      cpu->PC += 2;
      break;
    }
    case 0x4E: {
      // DONE
      uint8_t v = mmu->read8(cpu->HL);
      // printf("BIT1 (HL)\n");
      bit1_extended(v);
      mmu->write8(cpu->HL, v);
      break;
    }
    case 0x4F: {
      // DONE
      // printf("BIT1 A\n");
      bit1_extended(cpu->A);
      cpu->PC += 2;
      break;
    }
    case 0x50: {
      // DONE
      // printf("BIT2, B\n");
      bit2_extended(cpu->B);
      cpu->PC += 2;
      break;
    }
    case 0x51: {
      // DONE
      // printf("BIT2, C\n");
      bit2_extended(cpu->C);
      cpu->PC += 2;
      break;
    }
    case 0x52: {
      // DONE
      // printf("BIT2, D\n");
      bit2_extended(cpu->D);
      cpu->PC += 2;
      break;
    }
    case 0x53: {
      // DONE
      // printf("BIT2, E\n");
      bit2_extended(cpu->E);
      cpu->PC += 2;
      break;
    }
    case 0x54: {
      // DONE
      // printf("BIT2, H\n");
      bit2_extended(cpu->H);
      cpu->PC += 2;
      break;
    }
    case 0x55: {
      // DONE
      // printf("BIT2, L\n");
      bit2_extended(cpu->L);
      cpu->PC += 2;
      break;
    }
    case 0x56: {
      // DONE
      // printf("BIT2, L\n");
      uint8_t v = mmu->read8(cpu->HL);
      bit2_extended(v);
      mmu->write8(cpu->HL, v);
      cpu->PC += 2;
      break;
    }
    case 0x57: {
      // DONE
      // printf("BIT2 A\n");
      bit2_extended(cpu->A);
      cpu->PC += 2;
      break;
    }
    case 0x58: {
      // DONE
      // printf("BIT3 B\n");
      bit3_extended(cpu->B);
      cpu->PC += 2;
      break;
    }
    case 0x59: {
      // DONE
      // printf("BIT3 C\n");
      bit3_extended(cpu->C);
      cpu->PC += 2;
      break;
    }
    case 0x5A: {
      // DONE
      // printf("BIT3 D\n");
      bit3_extended(cpu->D);
      cpu->PC += 2;
      break;
    }
    case 0x5B: {
      // DONE
      // printf("BIT3 E\n");
      bit3_extended(cpu->E);
      cpu->PC += 2;
      break;
    }
    case 0x5C: {
      // DONE
      // printf("BIT3 H\n");
      bit3_extended(cpu->H);
      cpu->PC += 2;
      break;
    }
    case 0x5D: {
      // DONE
      // printf("BIT3 L\n");
      bit3_extended(cpu->L);
      cpu->PC += 2;
      break;
    }
    case 0x5E: {
      // DONE
      // printf("BIT3 L\n");
      uint8_t v = mmu->read8(cpu->HL);
      bit3_extended(v);
      mmu->write8(cpu->HL, v);
      cpu->PC += 2;
      break;
    }
    case 0x5F: {
      // DONE
      // printf("BIT3 A\n");
      bit3_extended(cpu->A);
      cpu->PC += 2;
      break;
    }
    case 0x60: {
      // DONE
      // printf("BIT4 B\n");
      bit4_extended(cpu->B);
      cpu->PC += 2;
      break;
    }
    case 0x61: {
      // DONE
      // printf("BIT4 C\n");
      bit4_extended(cpu->C);
      cpu->PC += 2;
      break;
    }
    case 0x62: {
      // DONE
      // printf("BIT4 D\n");
      bit4_extended(cpu->C);
      cpu->PC += 2;
      break;
    }
    case 0x63: {
      // DONE
      // printf("BIT4 E\n");
      bit4_extended(cpu->E);
      cpu->PC += 2;
      break;
    }
    case 0x64: {
      // DONE
      // printf("BIT4 H\n");
      bit4_extended(cpu->H);
      cpu->PC += 2;
      break;
    }
    case 0x65: {
      // DONE
      // printf("BIT4 L\n");
      bit4_extended(cpu->L);
      cpu->PC += 2;
      break;
    }
    case 0x66: {
      // DONE
      // printf("BIT4 L\n");
      uint8_t v = mmu->read8(cpu->HL);
      bit4_extended(v);
      mmu->write8(cpu->HL, v);
      cpu->PC += 2;
      break;
    }
    case 0x67: {
      // DONE
      // printf("BIT4 A\n");
      bit4_extended(cpu->A);
      cpu->PC += 2;
      break;
    }
    case 0x68: {
      // DONE
      // printf("BIT4 A\n");
      bit4_extended(cpu->A);
      cpu->PC += 2;
      break;
    }
    case 0x69: {
      // DONE
      // printf("BIT5 B\n");
      bit4_extended(cpu->B);
      cpu->PC += 2;
      break;
    }
    case 0x6A: {
      // DONE
      // printf("BIT5 C\n");
      bit4_extended(cpu->C);
      cpu->PC += 2;
      break;
    }
    case 0x6B: {
      // DONE
      // printf("BIT5 D\n");
      bit5_extended(cpu->D);
      cpu->PC += 2;
      break;
    }
    case 0x6C: {
      // DONE
      // printf("BIT5 E\n");
      bit5_extended(cpu->E);
      cpu->PC += 2;
      break;
    }
    case 0x6D: {
      // DONE
      // printf("BIT5 L\n");
      bit5_extended(cpu->L);
      cpu->PC += 2;
      break;
    }
    case 0x6E: {
      // DONE
      // printf("BIT5 L\n");
      uint8_t v = mmu->read8(cpu->HL);
      bit5_extended(v);
      mmu->write8(cpu->HL, v);
      cpu->PC += 2;
      break;
    }
    case 0x6F: {
      // DONE
      // printf("BIT5 A\n");
      bit5_extended(cpu->A);
      cpu->PC += 2;
      break;
    }
    case 0x70: {
      // DONE
      // printf("BIT6 A\n");
      bit6_extended(cpu->B);
      cpu->PC += 2;
      break;
    }
    case 0x71: {
      // DONE
      // printf("BIT6 C\n");
      bit6_extended(cpu->C);
      cpu->PC += 2;
      break;
    }
    case 0x72: {
      // DONE
      // printf("BIT6 D\n");
      bit6_extended(cpu->D);
      cpu->PC += 2;
      break;
    }
    case 0x73: {
      // DONE
      // printf("BIT6 E\n");
      bit6_extended(cpu->E);
      cpu->PC += 2;
      break;
    }
    case 0x74: {
      // DONE
      // printf("BIT6 H\n");
      bit6_extended(cpu->H);
      cpu->PC += 2;
      break;
    }
    case 0x75: {
      // DONE
      // printf("BIT6 L\n");
      bit6_extended(cpu->L);
      cpu->PC += 2;
      break;
    }
    case 0x76: {
      // DONE
      // printf("BIT6 L\n");
      uint8_t v = mmu->read8(cpu->HL);
      bit6_extended(v);
      mmu->write8(cpu->HL, v);
      cpu->PC += 2;
      break;
    }
    case 0x77: {
      // DONE
      // printf("BIT6 A\n");
      bit6_extended(cpu->A);
      cpu->PC += 2;
      break;
    }
    case 0x78: {
      // DONE
      // printf("BIT7 A\n");
      bit7_extended(cpu->B);
      cpu->PC += 2;
      break;
    }
    case 0x79: {
      // DONE
      // printf("BIT7 C\n");
      bit7_extended(cpu->C);
      cpu->PC += 2;
      break;
    }
    case 0x7A: {
      // DONE
      // printf("BIT7 D\n");
      bit7_extended(cpu->D);
      cpu->PC += 2;
      break;
    }
    case 0x7B: {
      // DONE
      // printf("BIT7 E\n");
      bit7_extended(cpu->E);
      cpu->PC += 2;
      break;
    }
    case 0x7C: {
      // DONE
      // printf("BIT7 H\n");
      bit7_extended(cpu->H);
      cpu->PC += 2;
      break;
    }
    case 0x7D: {
      // DONE
      // printf("BIT7 L\n");
      bit7_extended(cpu->L);
      cpu->PC += 2;
      break;
    }
    case 0x7E: {
      // DONE
      // printf("BIT7 L\n");
      uint8_t v = mmu->read8(cpu->HL);
      bit7_extended(v);
      mmu->write8(cpu->HL, v);
      cpu->PC += 2;
    }
    case 0x7F: {
      // DONE
      // printf("BIT7 A\n");
      bit7_extended(cpu->A);
      cpu->PC += 2;
      break;
    }
    case 0x80: {
      // DONE
      // printf("RES0 B\n");
      res0_extended(cpu->B);
      cpu->PC += 2;
      break;
    }
    case 0x81: {
      // DONE
      // printf("RES0 C\n");
      res0_extended(cpu->C);
      cpu->PC += 2;
      break;
    }
    case 0x82: {
      // DONE
      // printf("RES0 D\n");
      res0_extended(cpu->D);
      cpu->PC += 2;
      break;
    }
    case 0x83: {
      // DONE
      // printf("RES0 E\n");
      res0_extended(cpu->E);
      cpu->PC += 2;
      break;
    }
    case 0x84: {
      // DONE
      // printf("RES0 H\n");
      res0_extended(cpu->H);
      cpu->PC += 2;
      break;
    }
    case 0x85: {
      // DONE
      // printf("RES0 L\n");
      res0_extended(cpu->L);
      cpu->PC += 2;
      break;
    }
    case 0x86: {
      // DONE
      // printf("RES0 (HL)\n");
      uint8_t v = mmu->read8(cpu->HL);
      res0_extended(v);
      mmu->write8(cpu->HL, v);
      cpu->PC += 2;
      break;
    }
    case 0x87: {
      // DONE
      // printf("RES0 L\n");
      res0_extended(cpu->A);
      cpu->PC += 2;
      break;
    }
    case 0x88: {
      // DONE
      // printf("RES1 B\n");
      res1_extended(cpu->B);
      cpu->PC += 2;
      break;
    }
    case 0x89: {
      // DONE
      // printf("RES1 C\n");
      res1_extended(cpu->C);
      cpu->PC += 2;
      break;
    }
    case 0x8A: {
      // DONE
      // printf("RES1 D\n");
      res1_extended(cpu->D);
      cpu->PC += 2;
      break;
    }
    case 0x8B: {
      // DONE
      // printf("RES1 E\n");
      res1_extended(cpu->E);
      cpu->PC += 2;
      break;
    }
    case 0x8C: {
      // DONE
      // printf("RES1 E\n");
      res1_extended(cpu->H);
      cpu->PC += 2;
      break;
    }
    case 0x8D: {
      // DONE
      // printf("RES1 L\n");
      res1_extended(cpu->L);
      cpu->PC += 2;
      break;
    }
    case 0x8E: {
      // DONE
      // printf("RES1 (HL)\n");
      uint8_t v = mmu->read8(cpu->HL);
      res1_extended(v);
      mmu->write8(cpu->HL, v);
      cpu->PC += 2;
      break;
    }
    case 0x8F: {
      // DONE
      // printf("RES1 A\n");
      res1_extended(cpu->A);
      cpu->PC += 2;
      break;
    }
    case 0x90: {
      // DONE
      // printf("RES2 B\n");
      res2_extended(cpu->B);
      cpu->PC += 2;
      break;
    }
    case 0x91: {
      // DONE
      // printf("RES2 C\n");
      res2_extended(cpu->C);
      cpu->PC += 2;
      break;
    }
    case 0x92: {
      // DONE
      // printf("RES2 D\n");
      res2_extended(cpu->D);
      cpu->PC += 2;
      break;
    }
    case 0x93: {
      // DONE
      // printf("RES2 E\n");
      res2_extended(cpu->E);
      cpu->PC += 2;
      break;
    }
    case 0x94: {
      // DONE
      // printf("RES2 H\n");
      res2_extended(cpu->H);
      cpu->PC += 2;
      break;
    }
    case 0x95: {
      // DONE
      // printf("RES2 L\n");
      res2_extended(cpu->L);
      cpu->PC += 2;
      break;
    }

    case 0x96: {
      // DONE
      // printf("RES2 L\n");
      uint8_t v = mmu->read8(cpu->HL);
      res2_extended(v);
      mmu->write8(cpu->HL, v);
      cpu->PC += 2;
      break;
    }
    case 0x97: {
      // DONE
      // printf("RES2 A\n");
      res2_extended(cpu->A);
      cpu->PC += 2;
      break;
    }
    case 0x98: {
      // DONE
      // printf("RES2 B\n");
      res3_extended(cpu->B);
      cpu->PC += 2;
      break;
    }
    case 0x99: {
      // DONE
      // printf("RES2 C\n");
      res3_extended(cpu->C);
      cpu->PC += 2;
      break;
    }
    case 0x9A: {
      // DONE
      // printf("RES2 D\n");
      res3_extended(cpu->D);
      cpu->PC += 2;
      break;
    }
    case 0x9B: {
      // DONE
      // printf("RES2 E\n");
      res3_extended(cpu->E);
      cpu->PC += 2;
      break;
    }
    case 0x9C: {
      // DONE
      // printf("RES2 H\n");
      res3_extended(cpu->H);
      cpu->PC += 2;
      break;
    }
    case 0x9D: {
      // DONE
      // printf("RES2 L\n");
      res3_extended(cpu->L);
      cpu->PC += 2;
      break;
    }
    case 0x9E: {
      // DONE
      // printf("RES2 L\n");
      uint8_t v = mmu->read8(cpu->HL);
      res3_extended(v);
      mmu->write8(cpu->HL, v);
      cpu->PC += 2;
      break;
    }
    case 0x9F: {
      // DONE
      // printf("RES2 A\n");
      res3_extended(cpu->A);
      cpu->PC += 2;
      break;
    }
    case 0xA0: {
      // todo
      printf(" \n");
      break;
    }
    case 0xA1: {
      // todo
      printf(" \n");
      break;
    }
    case 0xA2: {
      // todo
      printf(" \n");
      break;
    }
    case 0xA3: {
      // todo
      printf(" \n");
      break;
    }
    case 0xA4: {
      // todo
      printf(" \n");
      break;
    }
    case 0xA5: {
      // todo
      printf(" \n");
      break;
    }
    case 0xA6: {
      // todo
      printf(" \n");
      break;
    }
    case 0xA7: {
      // todo
      printf(" \n");
      break;
    }
    case 0xA8: {
      // todo
      printf(" \n");
      break;
    }
    case 0xA9: {
      // todo
      printf(" \n");
      break;
    }
    case 0xAA: {
      // todo
      printf(" \n");
      break;
    }
    case 0xAB: {
      // todo
      printf(" \n");
      break;
    }
    case 0xAC: {
      // todo
      printf(" \n");
      break;
    }
    case 0xAD: {
      // todo
      printf(" \n");
      break;
    }
    case 0xAE: {
      // todo
      printf(" \n");
      break;
    }
    case 0xAF: {
      // todo
      printf(" \n");
      break;
    }
    case 0xB0: {
      // todo
      printf(" \n");
      break;
    }
    case 0xB1: {
      // todo
      printf(" \n");
      break;
    }
    case 0xB2: {
      // todo
      printf(" \n");
      break;
    }
    case 0xB3: {
      // todo
      printf(" \n");
      break;
    }
    case 0xB4: {
      // todo
      printf(" \n");
      break;
    }
    case 0xB5: {
      // todo
      printf(" \n");
      break;
    }
    case 0xB6: {
      // todo
      printf(" \n");
      break;
    }
    case 0xB7: {
      // todo
      printf(" \n");
      break;
    }
    case 0xB8: {
      // todo
      printf(" \n");
      break;
    }
    case 0xB9: {
      // todo
      printf(" \n");
      break;
    }
    case 0xBA: {
      // todo
      printf(" \n");
      break;
    }
    case 0xBB: {
      // todo
      printf(" \n");
      break;
    }
    case 0xBC: {
      // todo
      printf(" \n");
      break;
    }
    case 0xBD: {
      // todo
      printf(" \n");
      break;
    }
    case 0xBE: {
      // todo
      printf(" \n");
      break;
    }
    case 0xBF: {
      // todo
      printf(" \n");
      break;
    }
    case 0xC0: {
      // todo
      printf(" \n");
      break;
    }
    case 0xC1: {
      // todo
      printf(" \n");
      break;
    }
    case 0xC2: {
      // todo
      printf(" \n");
      break;
    }
    case 0xC3: {
      // todo
      printf(" \n");
      break;
    }
    case 0xC4: {
      // todo
      printf(" \n");
      break;
    }
    case 0xC5: {
      // todo
      printf(" \n");
      break;
    }
    case 0xC6: {
      // todo
      printf(" \n");
      break;
    }
    case 0xC7: {
      // todo
      printf(" \n");
      break;
    }
    case 0xC8: {
      // todo
      printf(" \n");
      break;
    }
    case 0xC9: {
      // todo
      printf(" \n");
      break;
    }
    case 0xCA: {
      // todo
      printf(" \n");
      break;
    }
    case 0xCB: {
      // todo
      printf(" \n");
      break;
    }
    case 0xCC: {
      // todo
      printf(" \n");
      break;
    }
    case 0xCD: {
      // todo
      printf(" \n");
      break;
    }
    case 0xCE: {
      // todo
      printf(" \n");
      break;
    }
    case 0xCF: {
      // todo
      printf(" \n");
      break;
    }
    case 0xD0: {
      // todo
      printf(" \n");
      break;
    }
    case 0xD1: {
      // todo
      printf(" \n");
      break;
    }
    case 0xD2: {
      // todo
      printf(" \n");
      break;
    }
    case 0xD3: {
      // todo
      printf(" \n");
      break;
    }
    case 0xD4: {
      // todo
      printf(" \n");
      break;
    }
    case 0xD5: {
      // todo
      printf(" \n");
      break;
    }
    case 0xD6: {
      // todo
      printf(" \n");
      break;
    }
    case 0xD7: {
      // todo
      printf(" \n");
      break;
    }
    case 0xD8: {
      // todo
      printf(" \n");
      break;
    }
    case 0xD9: {
      // todo
      printf(" \n");
      break;
    }
    }
  }
  case 0xCC: {
    // std::cout << "CALL nn" << std::endl;
    if (cpu->F & FLAG_ZERO) {
      execute_call();
    } else {
      cpu->PC = cpu->PC + 2;
    }
    break;
  }
  case 0xCD: {
    // std::cout << "CALL a16" << std::endl;
    //  uint16_t address =  mmu->read16(mmu->romData[cpu->PC +1]);
    //  cpu->PC = address;
    //  // bool carry_flag =  ((result >> 4) & 0x1);
    //  /
    //  //
    //  // bit:  7 6 5 4 3 2 1 0
    //  //       Z N H C 0 0 0 0
    //  cpu->F = ((cpu-F) << );
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0xCE: {
    // DONE:
    uint16_t n = mmu->romData[cpu->PC + 1];
    // printf("ADC A,u8 -- %X --\n", n);
    uint16_t _carry_flag = ((cpu->F >> 4) & 0x1);
    cpu->set_flag(FLAG_HALF_CARRY, (cpu->A & 0xf) + (n & 0xf) + _carry_flag > 0xf);
    uint16_t _result = cpu->A + n + _carry_flag;
    cpu->A = static_cast<uint8_t>(_result);

    cpu->set_flag(FLAG_ZERO, cpu->A == 0);
    cpu->set_flag(FLAG_SUBTRACT, 0);
    cpu->set_flag(FLAG_CARRY, _result > 0xff);

    // std::bitset<8> f_debug = cpu->F;

    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xCF: {
    // std::cout << "RST 08H" << std::endl;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xD0: {
    // std::cout << " RET NC" << std::endl;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xD3: {
    // std::cout << "  OUT(n), A" << std::endl;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xD4: {
    // std::cout << "  CALLNC, nn" << std::endl;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xD5: {
    // std::cout << " PUSH DE" << std::endl;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xD6: {
    // DONE:
    // sub(cpu->A, mmu->romData[cpu->PC + 1]);
    // printf("SUB d8. result of A -- %X -- 0xD6\n", cpu->A);
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xD7: {
    // std::cout << " RST 10H" << std::endl;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xD8: {
    // std::cout << " RET C" << std::endl;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xD9: {
    // std::cout << " RETI" << std::endl;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xDA: {
    // std::cout << "  JPC, nn" << std::endl;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xDB: {
    // std::cout << "   INA, (n)" << std::endl;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xDD: {
    // std::cout << "  CALLC, nn" << std::endl;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xDE: {
    // std::cout << " SBC nn" << std::endl;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xDF: {
    // std::cout << " RST 18H" << std::endl;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xE0: {
    cpu->A = mmu->romData[cpu->PC + 1];
    // printf("  LDH(n), A -- %X --\n", cpu->A);
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xE1: {
    // std::cout << " POP HL" << std::endl;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xE2: {
    // std::cout << "  LD(C), A" << std::endl;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xE3: {
    // std::cout << " XOR nn" << std::endl;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xE4: {
    // std::cout << " PUSH HL" << std::endl;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xE5: {
    // std::cout << " AND nn" << std::endl;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xE6: {
    // std::cout << " XOR nn" << std::endl;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xE7: {
    // std::cout << " RST 20H" << std::endl;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xE8: {
    // std::cout << "  ADDSP, r8" << std::endl;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xE9: {
    // std::cout << "  JP(HL)" << std::endl;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xEA: {
    // DONE:
    uint8_t l = mmu->romData[cpu->PC + 1];
    uint8_t h = mmu->romData[cpu->PC + 2];
    uint16_t nn = (h << 8) | l;
    // printf("LD(nn), A 0xEA nn: -- %X --\n", nn);
    mmu->write8(nn, cpu->A);
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0xEB: {
    // std::cout << " XOR nn" << std::endl;
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0xED: {
    // std::cout << " CALL nn" << std::endl;
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0xEE: {
    // std::cout << " XOR nn" << std::endl;
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0xEF: {
    // std::cout << " RST 28H" << std::endl;
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0xF0: {
    // std::cout << "  LDHA, (n)" << std::endl;
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0xF1: {
    // std::cout << " POP AF" << std::endl;
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0xF2: {
    // std::cout << "  LDA, (C)" << std::endl;
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0xF3: {
    // std::cout << " DI" << std::endl;
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0xF4: {
    // std::cout << " PUSH AF" << std::endl;
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0xF5: {
    // std::cout << " OR nn" << std::endl;
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0xF6: {
    // std::cout << " OR nn" << std::endl;
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0xF7: {
    // std::cout << " RST 30H" << std::endl;
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0xF8: {
    // std::cout << "  LD HL, SP+ r8" << std::endl;
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0xF9: {
    // std::cout << "  LDSP, HL" << std::endl;
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0xFA: {
    // std::cout << "  LD A, (nn)" << std::endl;
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0xFB: {
    // std::cout << " EI" << std::endl;
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0xFD: {
    // std::cout << " CALL nn" << std::endl;
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0xFE: {
    // DONE:
    uint8_t n = mmu->romData[cpu->PC + 1];
    // printf("CP A,n8 opcode:0xFE, n8:%X\n", n);
    cpu->set_flag(FLAG_ZERO, n == cpu->A);
    cpu->set_flag(FLAG_SUBTRACT, 1);
    cpu->set_flag(FLAG_HALF_CARRY, (cpu->A & 0x0F) < (n & 0x0F));
    cpu->set_flag(FLAG_CARRY, cpu->A < n);
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xFF: {
    // printf("RST 38H");
    cpu->PC = cpu->PC + 2;
    break;
  }
  default: {
    std::cerr << "Unknown opcode: 0x" << opcode << std::hex;
    break;
  }
  }
}

void InstructionSet::pop_(bool condition, uint16_t &reg) {
  uint8_t l = mmu->read8(cpu->SP & 0xFF);
  cpu->SP += 1;
  uint8_t h = mmu->read8((cpu->SP >> 8) & 0xFF);
  cpu->SP += 1;
  reg = (h << 8) | l;
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

void InstructionSet::or_(uint8_t &reg_1, uint8_t reg_2) {
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
    uint8_t l = mmu->read8(cpu->PC + 1);
    uint8_t h = mmu->read8(cpu->PC + 2);
    uint16_t t = (h << 8) | l;
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

void InstructionSet::and_(uint8_t &reg_1, uint8_t reg_2) {
  reg_1 = reg_1 & reg_2;
  cpu->set_flag(FLAG_ZERO, reg_1 == 0);
  cpu->set_flag(FLAG_HALF_CARRY, 1);
  cpu->clear_flag(FLAG_SUBTRACT);
  cpu->clear_flag(FLAG_CARRY);
}

void InstructionSet::sbc(uint8_t &reg_1, uint8_t reg_2) {
  uint8_t old = reg_1;
  uint8_t a = reg_1;
  uint8_t b = reg_2;
  bool c = (cpu->F & FLAG_CARRY);
  uint16_t result = a - b - c;

  reg_1 = result & 0xFF;
  cpu->set_flag(FLAG_ZERO, (reg_1 == 0));
  cpu->set_flag(FLAG_SUBTRACT, true);
  cpu->set_flag(FLAG_HALF_CARRY, (old & 0x0F) < (b & 0x0F) + c);
  cpu->set_flag(FLAG_CARRY, old < (reg_2 + c));
}
void InstructionSet::execute_call() {
  uint16_t address = mmu->read16(cpu->PC);
  cpu->PC += 2;
  cpu->SP -= 2;
  mmu->write16(cpu->SP, cpu->PC);
  cpu->PC = address;
}

void InstructionSet::xor_(uint8_t &reg_1, uint8_t reg_2) {
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
  reg = (reg << 1) | (msb >> 7);

  cpu->set_flag(FLAG_CARRY, msb);

  cpu->set_flag(FLAG_ZERO, reg == 0);

  cpu->clear_flag(FLAG_SUBTRACT);
  cpu->clear_flag(FLAG_HALF_CARRY);
}

void InstructionSet::rrca(uint8_t &reg) {
  // DONE
  bool least_sig_bit = reg & 1;
  reg = reg >> 1;
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

void InstructionSet::dec_mem(uint16_t &reg) {
  // To be DONE
  uint8_t tmp = mmu->read8(reg);
  uint8_t nibble_carry = tmp & 0x0F;
  tmp = tmp - 1;
  mmu->write8(reg, tmp);
}

void InstructionSet::adc(uint8_t &reg_1, uint8_t reg_2) {
  bool old_c = cpu->F & FLAG_CARRY;

  uint16_t c = reg_1 + reg_2 + old_c;
  cpu->set_flag(FLAG_ZERO, reg_1 == 0);
  cpu->clear_flag(FLAG_SUBTRACT);
  cpu->set_flag(FLAG_HALF_CARRY, ((reg_1 & 0x0F) + (reg_2 & 0x0F) + old_c) > 0x0F);
  cpu->set_flag(FLAG_CARRY, c > 0xFFF);
}

void InstructionSet::rra() {
  // DONE
  // Before: C = c, A = 76543210
  // After: A = c7654321, C = 0
  bool c = cpu->F & FLAG_CARRY;
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
  uint8_t old_a = cpu->A;
  uint8_t old_carry = cpu->F & FLAG_CARRY;
  uint8_t new_carry = (old_a >> 7) & 1;
  cpu->A = (old_a << 1) | old_carry;

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
  uint8_t result = reg_1 + reg_2;
  uint16_t _carry = reg_1 + reg_2;
  cpu->set_flag(FLAG_ZERO, result == 0);
  cpu->set_flag(FLAG_SUBTRACT, false);
  cpu->set_flag(FLAG_HALF_CARRY, (reg_1 & 0x0F) + (reg_2 & 0x0F) >= 0x10);
  cpu->set_flag(FLAG_CARRY, _carry >= 0x0100);
  return result;
}
void InstructionSet::sub(uint8_t &reg_1, uint8_t reg_2) {
  uint8_t a = reg_1;
  uint8_t b = reg_2;

  uint16_t result = a - b;

  reg_1 = result & 0xFF;

  cpu->set_flag(FLAG_ZERO, (reg_1 == 0));
  cpu->set_flag(FLAG_SUBTRACT, true);

  cpu->set_flag(FLAG_HALF_CARRY, (a & 0x0F) < (b & 0x0F));
  cpu->set_flag(FLAG_CARRY, a < b);
}
void InstructionSet::add16(uint16_t &destination, uint16_t &value) {
  // DONE
  uint32_t result = destination + value;
  cpu->clear_flag(FLAG_SUBTRACT);
  cpu->set_flag(FLAG_CARRY, result > 0xFFFF);
  cpu->set_flag(FLAG_HALF_CARRY, ((destination & 0x0FFF) + (value & 0x0FFF)) > 0x0FFF);
  destination = result & 0xFFFF;
}

void InstructionSet::inc(uint8_t &reg) {
  uint8_t old = reg;

  reg = old + 1;

  cpu->set_flag(FLAG_ZERO, reg == 0);
  cpu->clear_flag(FLAG_SUBTRACT);
  cpu->set_flag(FLAG_HALF_CARRY, (old & 0x0F) == 0x0F);
}

void InstructionSet::dec(uint8_t &reg) {
  // DONE

  uint8_t r = reg - 1;
  cpu->set_flag(FLAG_ZERO, r == 0);
  cpu->set_flag(FLAG_SUBTRACT, true);
  cpu->set_flag(FLAG_HALF_CARRY, (reg & 0x0F) == 0x00);
  reg = r;
}

void InstructionSet::inc_mem(uint16_t &reg) {
  // @brief this function increments
  // the value at the [reg],
  // and updates the F flag register
  // accordinly.
  uint8_t tmp = mmu->read8(reg);
  uint8_t nibble_carry = tmp & 0x0F;
  tmp = tmp + 1;
  printf("-- Debug: 0x%X to 0x%X --\n", tmp, reg);
  mmu->write8(reg, tmp);

  cpu->set_flag(FLAG_HALF_CARRY, (nibble_carry == 0x0F));
  cpu->set_flag(FLAG_ZERO, (tmp == 0));
  cpu->clear_flag(FLAG_SUBTRACT);
}

void InstructionSet::cpl(uint8_t &reg) {
  // DONE
  reg = ~reg;

  cpu->set_flag(FLAG_SUBTRACT, true);
  cpu->set_flag(FLAG_HALF_CARRY, true);
}
