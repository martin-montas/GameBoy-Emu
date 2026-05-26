// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
#include "instructions.hpp"
#include "MBC.hpp"
#include "MBC0.hpp"

#include <bitset>
#include <cstdio>
#include <iostream>
#include <stdint.h>

InstructionSet::InstructionSet(MMU *mmu, Cpu *cpu) {
  this->cpu = cpu;
  this->mmu = mmu;
}

void InstructionSet::ldr(uint16_t *reg) {
  // DONE
  uint8_t tmp_1 = this->mmu->romData[cpu->PC + 1];
  uint8_t tmp_2 = this->mmu->romData[cpu->PC + 2];
  uint16_t tmp = (tmp_2 << 8) | tmp_1;
  *reg = tmp;
}

void InstructionSet::ldr(uint8_t *reg, uint8_t address) {
  uint8_t tmp = this->mmu->romData[this->cpu->PC + 1];
  *reg = tmp;
}

void InstructionSet::ldr_mem(uint16_t *reg, uint8_t address) {
  uint16_t tmp = *reg;
  mmu->write8(address, tmp);
}

// finished these:
//
// 3E 41   ; LD A, $41    -> Puts ASCII 'A' into register A
// E0 01   ; LDH [$FF01], A -> Writes 'A' to the Serial Data register
// 3E 81   ; LD A, $81    -> Puts 0x81 into register A
// E0 02   ; LDH [$FF02], A -> Writes 0x81 to Serial Control (Triggers your
// hook!)
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
    ldr_mem(&cpu->BC, cpu->A);
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
    dec(cpu->B, 1);
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
    cpu->set_flag(FLAG_SUBTRACT,
                  ((cpu->HL & 0x0FFF) + (cpu->BC & 0x0FFF)) > 0x0FFF);
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
    dec(cpu->C, 1);
    // printf("DEC C -- %X --", cpu->C);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x0E: {
    // DONE
    // printf("LD C, d8 -- %X --\n", cpu->C);
    cpu->C = mmu->romData[cpu->PC + 1];
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x0F: {
    // DONE
    rrca(&cpu->A);
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
    ldr(&cpu->DE);
    // printf("LD (DE), d16 -- %X --\n", cpu->DE);
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0x12: {
    // DONE
    mmu->write8(mmu->read8(cpu->DE), cpu->A);
    // printf("LD (DE), A -- %X --\n", cpu->DE);
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
    dec(cpu->D, 1);
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
    // should be checked out
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
    dec(cpu->E, 1);
    // printf("DEC E -- %X --\n", cpu->E);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x1E: {
    // printf("LD E, d8");
    cpu->E = mmu->romData[cpu->PC++];
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x1F: {
    // printf("RRA\n");
    rra();
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x20: {
    // DONE:
    bool z = (cpu->F >> 7) & 1;
    // printf("JR NZ, r8 0x20 bool: %d\n", z);
    if (!z) {
      int8_t n = mmu->romData[cpu->PC + 1];
      cpu->PC = cpu->PC + 2 + n;
    } else {
      cpu->PC = cpu->PC + 2;
    }
    break;
  }
  case 0x21: {
    // DONE:
    ldr(&cpu->HL);
    // printf("LD HL, d16 0x21 -- (HL == %X) --\n", cpu->HL);
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0x22: {
    // std::cout << "LD (HL+), A" << std::endl;
    ldr_mem(&cpu->HL, cpu->A);
    cpu->HL++;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x23: {
    // DONE:
    // std::cout << "INC HL 0x23" << std::endl;
    cpu->HL = cpu->HL + 1;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x24: {
    // std::cout << "INC H" << std::endl;
    inc(cpu->H);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x25: {
    // soon
    // std::cout << "DEC H" << std::endl;
    // dec(cpu->H,1);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x26: {
    // std::cout << "LD H, d8" << std::endl;
    cpu->H = mmu->romData[cpu->PC++];
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x27: {
    // std::cout << "-- 0x27 -- DAA the jumped should be checked out!!"
    uint8_t correction = 0;
    if (!(cpu->F & FLAG_SUBTRACT)) {
      if ((cpu->A & 0x0F) > 9 || FLAG_HALF_CARRY) {
        correction |= 0x06;
      }
      if ((cpu->A > 0x99) || FLAG_HALF_CARRY) {
        correction |= 0x60;
        cpu->set_flag(FLAG_HALF_CARRY, true);
      }
    } else {
      if (FLAG_HALF_CARRY)
        correction |= 0x06;
      if (FLAG_CARRY)
        correction |= 0x60;
    }
    cpu->A += FLAG_SUBTRACT ? -correction : correction;
    cpu->set_flag(FLAG_ZERO, cpu->A == 0);
    cpu->set_flag(FLAG_HALF_CARRY, false);
    break;
  }
  case 0x28: {
    // DONE:
    // std::cout << "JR Z, r8 0x28" << std::endl;
    bool z = cpu->F & FLAG_ZERO;
    if (z) {
      // the offset might come from any reason of memory, should update
      int8_t offset = mmu->romData[cpu->PC + 1];
      cpu->PC = cpu->PC + 2 + offset;
    } else {
      cpu->PC = cpu->PC + 2;
    }
    break;
  }
  case 0x29: {
    // std::cout << "ADD HL, HL" << std::endl;
    add16(cpu->HL, cpu->HL);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x2A: {
    // std::cout << "LD A, (HL+)" << std::endl;
    cpu->A = mmu->read8(cpu->HL);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x2B: {
    // std::cout << "DEC HL" << std::endl;
    cpu->HL--;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x2C: {
    // std::cout << "INC L" << std::endl;
    inc(cpu->L);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x2D: {
    // std::cout << "DEC L" << std::endl;
    //  dec(cpu->L,1);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x2E: {
    // std::cout << "LD L, d8" << std::endl;
    cpu->L = mmu->romData[cpu->PC++];
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x2F: {
    // std::cout << "CPL" << std::endl;
    cpl(cpu->A);
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x30: { // JR NC,r8
    // DONE:
    bool c = (cpu->F >> 4) & 1;
    int8_t offset = static_cast<int8_t>(mmu->romData[cpu->PC + 1]);
    // printf("JR NC,r8 -- %d --\n", c);

    if (!c) {
      cpu->PC += 2 + offset;
    } else {
      cpu->PC += 2;
    }

    break;
  }
  case 0x31: {
    // std::cout << "LD SP, d16" << std::endl;
    ldr(&cpu->SP);
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0x32: {
    // std::cout << "LD (HL-), A" << std::endl;
    ldr_mem(&cpu->HL, cpu->A);
    cpu->HL--;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x33: {
    // std::cout << "INC SP" << std::endl;
    cpu->SP++;
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
    dec_mem(cpu->HL);
    // printf("DEC (HL) 0x35 0x%X \n", mmu->read8(cpu->HL));
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x36: {
    // std::cout << "LD (HL), d8" << std::endl;
    mmu->write8(cpu->HL, mmu->romData[cpu->PC++]);
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x37: {
    // DONE:
    // std::cout << "SCF" << std::endl;
    cpu->set_flag(FLAG_CARRY, true);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x38: {
    // DONE:
    // std::cout << "JR C, r8 0x38 DONE" << std::endl;
    if ((cpu->F >> 4) & 1) {
      uint8_t n = mmu->romData[cpu->PC] + 1;
      cpu->PC = cpu->PC + n;
    } else {
      cpu->PC = cpu->PC + 2;
    }
    break;
  }
  case 0x39: {
    // std::cout << "ADD HL, SP" << std::endl;
    add16(cpu->HL, cpu->SP);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x3A: {
    // std::cout << "LD A, (HL-)" << std::endl;
    cpu->A = mmu->read8(cpu->HL);
    cpu->HL--;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x3B: {
    // std::cout << "DEC SP" << std::endl;
    cpu->SP--;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x3C: {
    // std::cout << "INC A" << std::endl;
    inc(cpu->A);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x3D: {
    // std::cout << "DEC A TODO" << std::endl;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x3E: {
    // DONE:
    cpu->A = mmu->romData[cpu->PC + 1];
    // printf("LD A, d8: 0x3E A: -- %X -- \n", cpu->A);
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x3F: {
    // std::cout << "CPL" << std::endl;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x40: {
    // std::cout << "LD B, B" << std::endl;
    cpu->B = cpu->B;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x41: {
    // std::cout << "LD B, C" << std::endl;
    cpu->B = cpu->C;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x42: {
    // std::cout << "LD B, D" << std::endl;
    cpu->B = cpu->D;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x43: {
    // std::cout << "LD B, E" << std::endl;
    cpu->B = cpu->E;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x44: {
    // std::cout << "LD B, H" << std::endl;
    cpu->B = cpu->H;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x45: {
    // std::cout << "LD B, L" << std::endl;
    cpu->B = cpu->L;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x46: {
    // std::cout << "LD B, (HL)" << std::endl;
    cpu->B = mmu->romData[cpu->HL];
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x47: {
    // std::cout << "LD B, A" << std::endl;
    cpu->B = cpu->A;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x48: {
    // std::cout << "LD C, B" << std::endl;
    cpu->C = cpu->B;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x49: {
    // std::cout << "LD C, C" << std::endl;
    cpu->C = cpu->C;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x4A: {
    // std::cout << "LD C, D" << std::endl;
    cpu->C = cpu->D;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x4B: {
    // std::cout << "LD C, E" << std::endl;
    cpu->C = cpu->E;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x4C: {
    // std::cout << "LD C, H" << std::endl;
    cpu->C = cpu->H;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x4D: {
    // std::cout << "LD C, L" << std::endl;
    cpu->C = cpu->L;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x4E: {
    // std::cout << "LD C, (HL)" << std::endl;
    cpu->C = mmu->romData[cpu->HL];
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x4F: {
    // std::cout << "LD C, A" << std::endl;
    cpu->C = cpu->A;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x50: {
    // std::cout << "LD D, B" << std::endl;
    cpu->D = cpu->B;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x51: {
    // std::cout << "LD D, C" << std::endl;
    cpu->D = cpu->C;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x52: {
    // std::cout << "LD D, D" << std::endl;
    cpu->D = cpu->D;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x53: {
    // std::cout << "LD D, E" << std::endl;
    cpu->D = cpu->E;
    break;
  }
  case 0x54: {
    // std::cout << "LD D, H" << std::endl;
    cpu->D = cpu->H;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x55: {
    // std::cout << "LD D, L" << std::endl;
    cpu->D = cpu->L;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x56: {
    // std::cout << "LD D, (HL)" << std::endl;
    cpu->D = mmu->romData[cpu->HL];
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x57: {
    // std::cout << "LD D, A" << std::endl;
    cpu->D = cpu->A;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x58: {
    // std::cout << "LD E, B" << std::endl;
    cpu->E = cpu->B;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x59: {
    // std::cout << "LD E, C" << std::endl;
    cpu->E = cpu->C;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x5A: {
    // std::cout << "LD E, D" << std::endl;
    cpu->E = cpu->D;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x5B: {
    // std::cout << "LD E, E" << std::endl;
    cpu->E = cpu->E;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x5C: {
    // std::cout << "LD E, H" << std::endl;
    cpu->E = cpu->H;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x5D: {
    // std::cout << "LD E, L" << std::endl;
    cpu->E = cpu->L;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x5E: {
    // std::cout << "LD E, (HL)" << std::endl;
    cpu->E = mmu->romData[cpu->HL];
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x5F: {
    // std::cout << "LD E, A" << std::endl;
    cpu->E = cpu->A;
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0x60: {
    // std::cout << "LD H, B" << std::endl;
    cpu->H = cpu->B;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x61: {
    // std::cout << "LD H, C" << std::endl;
    cpu->H = cpu->C;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x62: {
    // std::cout << "LD H, D" << std::endl;
    cpu->H = cpu->D;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x63: {
    // std::cout << "LD H, E" << std::endl;
    cpu->H = cpu->E;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x64: {
    // std::cout << "LD H, H" << std::endl;
    cpu->H = cpu->H;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x65: {
    // std::cout << "LD H, L" << std::endl;
    cpu->H = cpu->L;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x66: {
    // std::cout << "LD H, (HL)" << std::endl;
    cpu->H = mmu->romData[cpu->HL];
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x67: {
    // std::cout << "LD H, A" << std::endl;
    cpu->H = cpu->A;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x68: {
    // std::cout << "LD L, B" << std::endl;
    cpu->L = cpu->B;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x69: {
    // std::cout << "LD L, C" << std::endl;
    cpu->L = cpu->C;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x6A: {
    // std::cout << "LD L, D" << std::endl;
    cpu->L = cpu->D;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x6B: {
    // std::cout << "LD L, E" << std::endl;
    cpu->L = cpu->E;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x6C: {
    // std::cout << "LD L, H" << std::endl;
    cpu->L = cpu->H;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x6D: {
    // std::cout << "LD L, L" << std::endl;
    cpu->L = cpu->L;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x6E: {
    // std::cout << "LD L, (HL)" << std::endl;
    cpu->L = mmu->romData[cpu->HL];
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x6F: {
    // std::cout << "LD L, A" << std::endl;
    cpu->L = cpu->A;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x70: {
    // std::cout << "LD (HL), B" << std::endl;
    mmu->romData[cpu->HL] = cpu->B;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x71: {
    // std::cout << "LD (HL), C" << std::endl;
    mmu->romData[cpu->HL] = cpu->C;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x72: {
    // std::cout << "LD (HL), D" << std::endl;
    mmu->romData[cpu->HL] = cpu->D;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x73: {
    // std::cout << "LD (HL), E" << std::endl;
    mmu->romData[cpu->HL] = cpu->E;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x74: {
    // std::cout << "LD (HL), H" << std::endl;
    mmu->romData[cpu->HL] = cpu->H;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x75: {
    // std::cout << "LD (HL), L" << std::endl;
    mmu->romData[cpu->HL] = cpu->L;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x76: {
    // std::cout << "HALT" << std::endl;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x77: {
    // DONE:
    mmu->write8(cpu->HL, cpu->A);
    cpu->PC = cpu->PC + 1;
    // printf("LD (HL), A: 0x%X 0x77\n", cpu->A);
    break;
  }
  case 0x78: {
    // std::cout << "LD A, B" << std::endl;
    cpu->A = cpu->B;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x79: {
    // std::cout << "LD A, C" << std::endl;
    cpu->A = cpu->C;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x7A: {
    // std::cout << "LD A, D" << std::endl;
    cpu->A = cpu->D;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x7B: {
    // std::cout << "LD A, E" << std::endl;
    cpu->A = cpu->E;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x7C: {
    // std::cout << "LD A, H" << std::endl;
    cpu->A = cpu->H;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x7D: {
    // std::cout << "LD A, L" << std::endl;
    cpu->A = cpu->L;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x7E: {
    // DONE:
    cpu->A = mmu->read8(cpu->HL);
    // printf("LD A, (HL) 0x%X 0x7E\n", cpu->A);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x7F: {
    // std::cout << "LD A, A" << std::endl;
    cpu->A = cpu->A;
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x80: {
    uint8_t r = add8(cpu->A, cpu->B);
    cpu->A = cpu->A + cpu->B;
    cpu->PC = cpu->PC + 1;
    // printf("ADD A, B: 0x80 %X\n", r);
    break;
  }
  case 0x81: {
    add8(cpu->A, cpu->C);
    cpu->PC = cpu->PC + 1;
    break;
  }

  case 0x82: {
    // std::cout << "ADD A, D" << std::endl;
    add8(cpu->A, cpu->D);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x83: {
    // std::cout << "ADD A, E" << std::endl;
    add8(cpu->A, cpu->E);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x84: {
    // std::cout << "ADD A, H" << std::endl;
    add8(cpu->A, cpu->H);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x85: {
    // std::cout << "ADD A, L" << std::endl;
    add8(cpu->A, cpu->L);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x86: {
    // std::cout << "ADD A, (HL)" << std::endl;
    add8_mem(cpu->A, mmu->romData[cpu->HL]);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x87: {
    // std::cout << "ADD A, A" << std::endl;
    add8(cpu->A, cpu->A);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x88: {
    // std::cout << "ADC A, B" << std::endl;
    //  adc(cpu->A, cpu->B);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x89: {
    // std::cout << "ADC A, C" << std::endl;
    //  adc(cpu->A, cpu->C);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x8A: {
    // std::cout << "ADC A, D" << std::endl;
    //  adc(cpu->A, cpu->D);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x8B: {
    // std::cout << "ADC A, E" << std::endl;
    //  adc(cpu->A, cpu->E);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x8C: {
    // std::cout << "ADC A, H" << std::endl;
    //  adc(cpu->A, cpu->H);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x8D: {
    // std::cout << "ADC A, L" << std::endl;
    //  adc(cpu->A, cpu->L);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x8E: {
    // std::cout << "ADC A, (HL)" << std::endl;
    //  adc(cpu->A, mmu->romData[cpu->HL]);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x8F: {
    // std::cout << "ADC A, A" << std::endl;
    //  adc(cpu->A, cpu->A);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x90: {
    // std::cout << "SUB B" << std::endl;
    sub(&cpu->A, &cpu->B);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x91: {
    // std::cout << "SUB C" << std::endl;
    sub(&cpu->A, &cpu->C);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x92: {
    // std::cout << "SUB D" << std::endl;
    sub(&cpu->A, &cpu->D);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x93: {
    // std::cout << "SUB E" << std::endl;
    sub(&cpu->A, &cpu->E);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x94: {
    // std::cout << "SUB H" << std::endl;
    sub(&cpu->A, &cpu->H);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x95: {
    // std::cout << "SUB L" << std::endl;
    sub(&cpu->A, &cpu->L);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x96: {
    // std::cout << "SUB (HL)" << std::endl;
    sub(&cpu->A, &mmu->romData[cpu->HL]);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x97: {
    // std::cout << "SUB A" << std::endl;
    sub(&cpu->A, &cpu->A);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x98: {
    // std::cout << "SBC A, B" << std::endl;
    //  sbc(&cpu->A, &cpu->B);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x99: {
    // std::cout << "SBC A, C" << std::endl;
    //  sbc(&cpu->A, &cpu->C);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x9A: {
    // std::cout << "SBC A, D" << std::endl;
    //  sbc(&cpu->A, &cpu->D);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x9B: {
    // std::cout << "SBC A, E" << std::endl;
    //  sbc(&cpu->A, &cpu->E);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x9C: {
    // std::cout << "SBC A, H" << std::endl;
    //  sbc(&cpu->A, &cpu->H);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x9D: {
    // std::cout << "SBC A, L" << std::endl;
    //  sbc(&cpu->A, &cpu->L);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x9E: {
    // std::cout << "SBC A, (HL)" << std::endl;
    //  sbc(&cpu->A, &mmu->romData[cpu->HL]);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0x9F: {
    // std::cout << "SBC A, A" << std::endl;
    //  sbc(&cpu->A, &cpu->A);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xA0: {
    // std::cout << "AND B" << std::endl;
    //  and_(&cpu->A, &cpu->B);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xA1: {
    // std::cout << "AND C" << std::endl;
    //  and_(&cpu->A, &cpu->C);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xA2: {
    // std::cout << "AND D" << std::endl;
    //  and_(&cpu->A, &cpu->D);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xA3: {
    // std::cout << "AND E" << std::endl;
    //  and_(&cpu->A, &cpu->E);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xA4: {
    // std::cout << "AND H" << std::endl;
    //  and_(&cpu->A, &cpu->H);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xA5: {
    // std::cout << "AND L" << std::endl;
    //  and_(&cpu->A, &cpu->L);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xA6: {
    // std::cout << "AND (HL)" << std::endl;
    //  and_(&cpu->A, &mmu->romData[cpu->HL]);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xA7: {
    // std::cout << "AND A" << std::endl;
    //  and_(&cpu->A, &cpu->A);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xA8: {
    // std::cout << "XOR B" << std::endl;
    //  xor_(&cpu->A, &cpu->B);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xA9: {
    // std::cout << "XOR C" << std::endl;
    //  xor_(&cpu->A, &cpu->C);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xAA: {
    // std::cout << "XOR D" << std::endl;
    //  xor_(&cpu->A, &cpu->D);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xAB: {
    // std::cout << "XOR E" << std::endl;
    //  xor_(&cpu->A, &cpu->E);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xAC: {
    // std::cout << "XOR H" << std::endl;
    //  xor_(&cpu->A, &cpu->H);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xAD: {
    // std::cout << "XOR L" << std::endl;
    //  xor_(&cpu->A, &cpu->L);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xAE: {
    // std::cout << "XOR (HL)" << std::endl;
    //  xor_(&cpu->A, &mmu->romData[cpu->HL]);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xAF: {
    // std::cout << "XOR A" << std::endl;
    //  xor_(&cpu->A, &cpu->A);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xB0: {
    // std::cout << "OR B" << std::endl;
    or_(&cpu->A, &cpu->B);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xB1: {
    // std::cout << "OR C" << std::endl;
    or_(&cpu->A, &cpu->C);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xB2: {
    // std::cout << "OR D" << std::endl;
    or_(&cpu->A, &cpu->D);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xB3: {
    // std::cout << "OR E" << std::endl;
    or_(&cpu->A, &cpu->E);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xB4: {
    // std::cout << "OR H" << std::endl;
    or_(&cpu->A, &cpu->H);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xB5: {
    // std::cout << "OR L" << std::endl;
    or_(&cpu->A, &cpu->L);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xB6: {
    // std::cout << "OR (HL)" << std::endl;
    or_(&cpu->A, &mmu->romData[cpu->HL]);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xB7: {
    // std::cout << "OR A" << std::endl;
    or_(&cpu->A, &cpu->A);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xB8: {
    // std::cout << "CP B" << std::endl;
    cp_(&cpu->A, &cpu->B);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xB9: {
    // std::cout << "CP C" << std::endl;
    cp_(&cpu->A, &cpu->C);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xBA: {
    // std::cout << "CP D" << std::endl;
    cp_(&cpu->A, &cpu->D);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xBB: {
    // std::cout << "CP D" << std::endl;
    cp_(&cpu->A, &cpu->E);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xBC: {
    // std::cout << "CP H" << std::endl;
    cp_(&cpu->A, &cpu->H);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xBD: {
    // std::cout << "CP L" << std::endl;
    cp_(&cpu->A, &cpu->L);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xBE: {
    // std::cout << "CP (HL)" << std::endl;
    cp_(&cpu->A, &mmu->romData[cpu->HL]);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xBF: {
    // std::cout << "CP A" << std::endl;
    cp_(&cpu->A, &cpu->A);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xC0: {
    // std::cout << "RET NZ" << std::endl;
    if (!(cpu->F & FLAG_ZERO)) {
      cpu->PC = cpu->PC + 1;
      ret(true);
    }
    break;
  }
  case 0xC1: {
    // std::cout << "POP BC" << std::endl;
    cpu->C = mmu->read8(cpu->SP);
    cpu->B = mmu->read8(cpu->SP + 1);
    cpu->SP += 1;
    break;
  }
  case 0xC2: {
    // std::cout << "JP NZ, nn" << std::endl;
    if (!(cpu->F & FLAG_ZERO)) {
      cpu->PC = mmu->read8(cpu->PC) | mmu->read8(cpu->PC + 1) << 8;
    } else {
      cpu->PC += 3;
    }
    break;
  }
  case 0xC3: {
    // DONE:
    auto _lw = mmu->read8(cpu->PC + 1);
    auto _hi = mmu->read8(cpu->PC + 2);
    uint16_t jmp_addr = (_hi << 8) | _lw;

    // printf("JP u16: 0xC3\n");
    cpu->PC = jmp_addr;
    break;
  }
  case 0xC4: {
    // std::cout << "CALL nn 0xc4 TODO" << std::endl;
    if (!(cpu->F & FLAG_ZERO)) {
      call(true);
    }
    break;
  }
  case 0xC5: {
    // std::cout << "PUSH BC" << std::endl;
    cpu->SP -= 2;
    mmu->write8(cpu->SP + 1, cpu->B);
    mmu->write8(cpu->SP, cpu->C);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xC6: {
    // std::cout << "ADD A, d8" << std::endl;
    add8(cpu->A, mmu->read8(cpu->PC + 1));
    cpu->PC = cpu->PC + 2;
    break;
  }
  case 0xC7: {
    // std::cout << "RST 00h" << std::endl;
    cpu->SP -= 2;
    mmu->write8(cpu->SP, (cpu->PC >> 8) & 0xFF);
    mmu->write8(cpu->SP + 1, cpu->PC & 0xFF);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xC8: {
    // std::cout << "RET Z TODO" << std::endl;
    if (cpu->F & FLAG_ZERO) {
      ret(true);
    }
    break;
  }
  case 0xC9: {
    // std::cout << "RET" << std::endl;
    ret(true);
    cpu->PC = cpu->PC + 1;
    break;
  }
  case 0xCA: {
    // std::cout << "JP Z, nn 0xCA" << std::endl;
    if (cpu->F & FLAG_ZERO) {
      cpu->PC = mmu->read16(cpu->PC + 1);
    }
    break;
  }
  case 0xCB: {
    // std::cout << " ---  PREFIX CB NOTDONE 0xCB -- " << std::endl;
    //         switch (mmu->read8(cpu->PC + 1)) {
    //                 case 0x00:
    //                         //rlc_extended(&cpu->B);
    //                         break;
    //                 case 0x01:  // rlc c
    //                             ////std::cout << "rlc c" << std::endl;
    //                             //rlc_extended(&cpu->C);
    //                         break;
    //                 case 0x02:  // rlc d
    //                             ////std::cout << "rlc d" << std::endl;
    //                             //rlc_extended(&cpu->D);
    //                         break;
    //                 case 0x03:  // RLC E
    //                             ////std::cout << "RLC E" << std::endl;
    //                             //rlc_extended(&cpu->E);
    //                         break;
    //                 case 0x04:  // RLC H
    //                             ////std::cout << "RLC H" << std::endl;
    //                             //rlc_extended(&cpu->H);
    //                         break;
    //                 case 0x05:  // RLC L
    //                             ////std::cout << "RLC L" << std::endl;
    //                             //rlc_extended(&cpu->L);
    //                         break;
    //                 case 0x06:  // RLC (HL)
    //                             ////std::cout << "RLC (HL)" << std::endl;
    //                             // TODO(martin-montas)
    //                             //rlc_extended_mem(&cpu->HL);
    //                         break;
    //                 case 0x07:  // RLC A
    //                             ////std::cout << "RLC A" << std::endl;
    //                             //rlc_extended(&cpu->A);
    //                         break;
    //                 case 0x08:  // RRC B
    //                             ////std::cout << "RRC B" << std::endl;
    //                             //rcc_extended(&cpu->B);
    //                         break;
    //                 case 0x09:  // RRC C
    //                             ////std::cout << "RRC C" << std::endl;
    //                             //rcc_extended(&cpu->C);
    //                         break;
    //                 case 0x0A:  // RRC D
    //                             ////std::cout << "RRC D" << std::endl;
    //                             //rcc_extended(&cpu->D);
    //                         break;
    //                 case 0x0B:  // RRC E
    //                             ////std::cout << "RRC E" << std::endl;
    //                             //rcc_extended(&cpu->E);
    //                         break;
    //                 case 0x0C:  // RRC H
    //                             ////std::cout << "RRC H" << std::endl;
    //                             //rcc_extended(&cpu->H);
    //                         break;
    //                 case 0x0D:  // RRC L
    //                             ////std::cout << "RRC L" << std::endl;
    //                             //rcc_extended(&cpu->L);
    //                         break;
    //                 case 0x0E:  // RRC (HL)
    //                         //std::cout << "RRC (HL)" << std::endl;
    //                         // TODO(martin-montas)
    //                         break;
    //                 case 0x0F:  // RRC A
    //                             ////std::cout << "RRC A" << std::endl;
    //                             //rcc_extended(&cpu->A);
    //                         break;
    //                 case 0x10:  // RL B
    //                         //std::cout << "RL B " << std::endl;
    //                         //rl_extended(&cpu->B);
    //                         break;
    //                 case 0x11:  // RL C
    //                         //std::cout << "RL C" << std::endl;
    //                         rl_extended(&cpu->A);
    //                         break;
    //                 case 0x12:  // RL D
    //                         //std::cout << "RL D" << std::endl;
    //                         rl_extended(&cpu->A);
    //                         break;
    //                 case 0x13:  // RL E
    //                         //std::cout << "RL E" << std::endl;
    //                         rl_extended(&cpu->A);
    //                         break;
    //                 case 0x14:  // RL H
    //                         //std::cout << "RL H" << std::endl;
    //                         rl_extended(&cpu->A);
    //                         break;
    //                 case 0x15:  //  RL L
    //                         //std::cout << "RL L " << std::endl;
    //                         rl_extended(&cpu->A);
    //                         break;
    //                 case 0x16:  // RL (HL)
    //                         //std::cout << "RL (HL)" << std::endl;
    //                         // TODO(martin-montas)
    //                         break;
    //                 case 0x17:  // RL A
    //                         //std::cout << "RL A" << std::endl;
    //                         rl_extended(&cpu->A);
    //                         break;
    //                 case 0x19:  // RR B
    //                         rr_extended(&cpu->B);
    //                         //std::cout << "RR B" << std::endl;
    //                         break;
    //                 case 0x1A:  // RR C
    //                         rr_extended(&cpu->C);
    //                         //std::cout << "RR C" << std::endl;
    //                         break;
    //                 case 0x1B:  // RR D
    //                         rr_extended(&cpu->D);
    //                         //std::cout << "RR D" << std::endl;
    //                         break;
    //                 case 0x1C:  // RR E
    //                         rr_extended(&cpu->E);
    //                         //std::cout << "RR E" << std::endl;
    //                         break;
    //                 case 0x1D:  // RR H
    //                         rr_extended(&cpu->H);
    //                         //std::cout << "RR H" << std::endl;
    //                         break;
    //                 case 0x1E:  // RR L
    //                         rr_extended(&cpu->L);
    //                         //std::cout << "RR L" << std::endl;
    //                         break;
    //                 case 0x1F:  // RR A
    //                         //std::cout << "RR A" << std::endl;
    //                         rr_extended(&cpu->A);
    //                         break;
    //                 case 0x20:  // SLA B
    //                         //std::cout << "SLA B" << std::endl;
    //                         sla_extended(&cpu->B);
    //                         break;
    //                 case 0x21:  // SLA C
    //                         //std::cout << "SLA C" << std::endl;
    //                         sla_extended(&cpu->C);
    //                         break;
    //                 case 0x22:  // SLA D
    //                         //std::cout << "SLA D" << std::endl;
    //                         sla_extended(&cpu->D);
    //                         break;
    //                 case 0x23:  // SLA E
    //                         //std::cout << "SLA E" << std::endl;
    //                         sla_extended(&cpu->E);
    //                         break;
    //                 case 0x24:  // SLA H
    //                         //std::cout << "SLA H" << std::endl;
    //                         sla_extended(&cpu->H);
    //                         break;
    //                 case 0x25:  // SLA L
    //                         //std::cout << "SLA L" << std::endl;
    //                         sla_extended(&cpu->L);
    //                         break;
    //                 case 0x26:  // SLA (HL)
    //                         //std::cout << "SLA (HL)" << std::endl;
    //                         // TODO(martin-montas)
    //                         break;
    //                 case 0x27:  // SLA A
    //                         //std::cout << "SLA A" << std::endl;
    //                         sla_extended(&cpu->A);
    //                         break;
    //                 case 0x29:  // SRA B
    //                         //std::cout << "SRA B" << std::endl;
    //                         sra_extended(&cpu->B);
    //                         break;
    //                 case 0x2A:  // SRA C
    //                         //std::cout << "SRA C" << std::endl;
    //                         sra_extended(&cpu->C);
    //                         break;
    //                 case 0x2B:  // SRA D
    //                         //std::cout << "SRA D" << std::endl;
    //                         sra_extended(&cpu->D);
    //                         break;
    //                 case 0x2C:  // SRA E
    //                         //std::cout << "SRA E" << std::endl;
    //                         sra_extended(&cpu->E);
    //                         break;
    //                 case 0x2D:  // SRA H
    //                         //std::cout << "SRA H" << std::endl;
    //                         sra_extended(&cpu->H);
    //                         break;
    //                 case 0x2E:  // SRA L
    //                         //std::cout << "SRA L" << std::endl;
    //                         sra_extended(&cpu->L);
    //                         break;
    //                 case 0x2F:  // SRA A
    //                         //std::cout << "SRA A" << std::endl;
    //                         sra_extended(&cpu->A);
    //                         break;
    //                 case 0x30:  // SWAP B
    //                         //std::cout << "SWAP B" << std::endl;
    //                         swap_extended(&cpu->B);
    //                         break;
    //                 case 0x31:  // SWAP C
    //                         //std::cout << "SWAP C" << std::endl;
    //                         swap_extended(&cpu->C);
    //                         break;
    //                 case 0x32:  // SWAP D
    //                         //std::cout << "SWAP D" << std::endl;
    //                         swap_extended(&cpu->D);
    //                         break;
    //                 case 0x33:  // SWAP E
    //                         //std::cout << "SWAP E" << std::endl;
    //                         swap_extended(&cpu->E);
    //                         break;
    //                 case 0x34:  // SWAP H
    //                         //std::cout << "SWAP H" << std::endl;
    //                         break;
    //                 case 0x35:  // SWAP L
    //                         //std::cout << "SWAP L" << std::endl;
    //                         swap_extended(&cpu->L);
    //                 case 0x36:  // SWAP (HL)
    //                         //std::cout << "SWAP (HL)" << std::endl;
    //                         // TODO(martin-montas)
    //                         break;
    //                 case 0x37:  // SWAP A
    //                         //std::cout << "SWAP A" << std::endl;
    //                         swap_extended(&cpu->A);
    //                         break;
    //                 case 0x39:  // SRL B
    //                         //std::cout << "SRL B" << std::endl;
    //                         srl_extended(&cpu->B);
    //                         break;
    //                 case 0x3A:  // SRL C
    //                         //std::cout << "SRL C" << std::endl;
    //                         srl_extended(&cpu->C);
    //                         break;
    //                 case 0x3B:  // SRL D
    //                         //std::cout << "SRL D" << std::endl;
    //                         srl_extended(&cpu->D);
    //                         break;
    //                 case 0x3C:  // SRL E
    //                         //std::cout << "SRL E" << std::endl;
    //                         srl_extended(&cpu->E);
    //                         break;
    //                 case 0x3D:  // SRL H
    //                         //std::cout << "SRL H" << std::endl;
    //                         srl_extended(&cpu->H);
    //                         break;
    //                 case 0x3E:  // SRL L
    //                         //std::cout << "SRL L " << std::endl;
    //                         srl_extended(&cpu->L);
    //                         break;
    //                 case 0x3F:  // SRL A
    //                         //std::cout << "SRL A" << std::endl;
    //                         srl_extended(&cpu->A);
    //                         break;
    //                 default:
    //                         //std::cerr << "Unknown opcode: 0x"
    //                                 << //std::hex <<
    //                                 static_cast<int>(mmu->read8(cpu->PC +
    //                                 1))
    //                                 << //std::endl;
    //                         break;
    //         }
    //  Breaks out of the switch block:
    break;
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
    // //std::cout << "CALL a16" << std::endl;
    // uint16_t address =  mmu->read16(mmu->romData[cpu->PC +1]);
    // cpu->PC = address;
    // // bool carry_flag =  ((result >> 4) & 0x1);
    // /
    // //
    // // bit:  7 6 5 4 3 2 1 0
    // //       Z N H C 0 0 0 0
    // cpu->F = ((cpu-F) << );
    cpu->PC = cpu->PC + 3;
    break;
  }
  case 0xCE: {
    // DONE:
    uint16_t n = mmu->romData[cpu->PC + 1];
    // printf("ADC A,u8 -- %X --\n", n);
    uint16_t _carry_flag = ((cpu->F >> 4) & 0x1);
    cpu->set_flag(FLAG_HALF_CARRY,
                  (cpu->A & 0xf) + (n & 0xf) + _carry_flag > 0xf);
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
    sub(&cpu->A, &mmu->romData[cpu->PC + 1]);
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
    // std::cout << "  LDH(n), A" << std::endl;
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
    // std::cout << " RST 38H" << std::endl;
    cpu->PC = cpu->PC + 2;
    break;
  }
  default: {
    std::cerr << "Unknown opcode: 0x" << std::hex;
    break;
  }
  }
}

void InstructionSet::ret(bool condition) {
  if (condition) {
    cpu->SP += 2;
    uint16_t address = mmu->read16(cpu->SP);
    cpu->PC = address;
  }
}

void InstructionSet::or_(uint8_t *reg_1, uint8_t *reg_2) {
  uint8_t tmp = *reg_1 | *reg_2;
  cpu->set_flag(FLAG_ZERO, (tmp == 0));
  cpu->clear_flag(FLAG_SUBTRACT);
  cpu->clear_flag(FLAG_HALF_CARRY);
  cpu->clear_flag(FLAG_CARRY);
  *reg_1 = tmp;
}

void InstructionSet::cpl(uint8_t reg) {
  reg = ~reg;

  cpu->set_flag(FLAG_SUBTRACT, true);
  cpu->set_flag(FLAG_HALF_CARRY, true);
}

void InstructionSet::inc_mem(uint16_t reg) {
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

void InstructionSet::inc(uint8_t &reg) {
  uint8_t nibble_carry = reg & 0xF;

  reg = reg + 1;
  cpu->set_flag(FLAG_HALF_CARRY, (nibble_carry == 0x0F));

  cpu->set_flag(FLAG_ZERO, (reg == 0));
  cpu->clear_flag(FLAG_SUBTRACT);
}

void InstructionSet::inc(uint16_t *reg) {
  // uint8_t nibble_carry = reg & 0x0F;
  *reg = *reg + 1;
  // cpu->set_flag(FLAG_HALF_CARRY, (nibble_carry == 0x0F));

  // cpu->set_flag(FLAG_ZERO, (reg == 0));
  // cpu->clear_flag(FLAG_SUBTRACT);
}

void InstructionSet::dec(uint8_t &reg, uint8_t n) {
  // DONE
  uint8_t r = reg - n;
  cpu->set_flag(FLAG_ZERO, r == 0);
  cpu->set_flag(FLAG_SUBTRACT, true);
  cpu->set_flag(FLAG_HALF_CARRY, (reg & 0x0F) == 0);
  reg = reg - n;
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

void InstructionSet::add16(uint16_t &destination, uint16_t &value) {
  // you are here
  uint32_t result = destination + value;
  cpu->clear_flag(FLAG_SUBTRACT);
  cpu->set_flag(FLAG_CARRY, result > 0xFFFF);
  cpu->set_flag(FLAG_HALF_CARRY,
                ((destination & 0x0FFF) + (value & 0x0FFF)) > 0x0FFF);
  destination = result & 0xFFFF;
}

void ldhl(int8_t value) {}
// TODO(martin-montas)
// void InstructionSet::adc(uint8_t& reg_1, uint8_t reg_2) {
//     uint8_t carry = cpu->get_flag(FLAG_CARRY) ? 1 : 0;
//     uint16_t result = reg_1 + reg_2 + carry;
//     reg_1 = result & 0xFF;
//     cpu->set_flag(FLAG_ZERO, (reg_1 == 0));
//     cpu->set_flag(FLAG_HALF_CARRY, ((reg_1 & 0x0F)
//     + (reg_2 & 0x0F) + carry) > 0x0F);
//     cpu->set_flag(FLAG_CARRY, (result > 0xFF));
//     cpu->clear_flag(FLAG_SUBTRACT);
// }

// TODO(martin-montas)
// void sbc(uint8_t reg_1, uint8_t reg_2) {
//     uint8_t carry = cpu->get_flag(FLAG_CARRY) ? 1 : 0;
//     uint16_t result = reg_1 - reg_2 - carry;
//     reg_1 = result & 0xFF;
//     cpu->set_flag(FLAG_ZERO, (reg_1 == 0));
//     cpu->set_flag(FLAG_SUBTRACT, 1);
//     cpu->set_flag(FLAG_HALF_CARRY,
//     ((reg_1 & 0x0F) < (reg_2 & 0x0F) + carry));
//     cpu->set_flag(FLAG_CARRY, (result > 0xFF));
// }

void InstructionSet::sub(uint8_t *reg_1, uint8_t *reg_2) {
  uint8_t a = *reg_1;
  uint8_t b = *reg_2;

  uint16_t result = a - b;

  *reg_1 = result & 0xFF;

  cpu->set_flag(FLAG_ZERO, (*reg_1 == 0));
  cpu->set_flag(FLAG_SUBTRACT, true);

  cpu->set_flag(FLAG_HALF_CARRY, (a & 0x0F) < (b & 0x0F));
  cpu->set_flag(FLAG_CARRY, a < b);
}

void InstructionSet::rlc(uint8_t reg) {
  bool msb = reg & 0x80;
  reg = (reg << 1) | (msb >> 7);

  cpu->set_flag(FLAG_CARRY, msb);

  cpu->set_flag(FLAG_ZERO, reg == 0);

  cpu->clear_flag(FLAG_SUBTRACT);
  cpu->clear_flag(FLAG_HALF_CARRY);
}

void InstructionSet::rrca(uint8_t *reg) {
  // DONE
  bool least_sig_bit = *reg & 1;
  *reg = *reg >> 1;
  if (least_sig_bit) {
    *reg |= 128;
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

void InstructionSet::dec_mem(uint16_t reg) {
  // To be DONE
  uint8_t tmp = mmu->read8(reg);
  uint8_t nibble_carry = tmp & 0x0F;
  tmp = tmp - 1;
  mmu->write8(reg, tmp);

  cpu->set_flag(FLAG_HALF_CARRY, nibble_carry == 0);
  cpu->set_flag(FLAG_ZERO, tmp == 0);
  cpu->set_flag(FLAG_SUBTRACT, true);
}

void InstructionSet::rla() {
  // DONE but should check
  printf("RLA\n");
  bool carry = cpu->F & FLAG_CARRY;
  uint16_t old_bit = (cpu->A >> 7) & 1;
  cpu->A = cpu->A << 1;
  cpu->set_flag(FLAG_CARRY, cpu->A & 0x80);
  if (carry) {
    cpu->A |= 0x01;
  }
  cpu->clear_flag(FLAG_SUBTRACT);
  cpu->clear_flag(FLAG_HALF_CARRY);
  cpu->clear_flag(FLAG_ZERO);
}

void InstructionSet::add8_mem(uint8_t destination, uint8_t value) {
  mmu->write8(destination, destination + value);
  cpu->set_flag(FLAG_ZERO, (destination + value) == 0);
  cpu->clear_flag(FLAG_SUBTRACT);
  cpu->set_flag(FLAG_HALF_CARRY,
                ((destination & 0x0F) + (value & 0x0F)) > 0x0F);
  cpu->set_flag(FLAG_CARRY, (destination + value) > 0xFF);
}

void InstructionSet::rra() {
  bool msb = cpu->A & 0x01;
  cpu->A = cpu->A >> 1;
  if (msb) {
    cpu->A |= 0x80;
  }

  cpu->clear_flag(FLAG_ZERO);
  cpu->clear_flag(FLAG_SUBTRACT);
  cpu->clear_flag(FLAG_HALF_CARRY);
  cpu->set_flag(FLAG_CARRY, msb);
}
void dec_mem(uint8_t *value) { return; }

void InstructionSet::cp_(uint8_t *reg_1, uint8_t *reg_2) {
  uint16_t tmp = reg_1 - reg_2;
  cpu->set_flag(FLAG_ZERO, (reg_1 == reg_2));
  cpu->set_flag(FLAG_SUBTRACT, 1);
  cpu->set_flag(FLAG_HALF_CARRY, ((*reg_1 & 0x0F) < (*reg_2 & 0x0F)));
  cpu->set_flag(FLAG_CARRY, (tmp > 0xFF));
}

void InstructionSet::call(bool condition) {
  if (condition) {
    uint16_t address = mmu->read8(cpu->PC + 1);
    cpu->PC += 3;
    cpu->SP -= 2;
    mmu->write16(cpu->SP, cpu->PC);
    cpu->PC = address;
  } else {
    cpu->PC += 3;
  }
}

void InstructionSet::execute_call() {
  uint16_t address = mmu->read16(cpu->PC);
  cpu->PC += 2;
  cpu->SP -= 2;
  mmu->write16(cpu->SP, cpu->PC);
  cpu->PC = address;
}
