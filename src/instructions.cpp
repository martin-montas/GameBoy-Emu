// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
//
#include <cstdint>
#include <iostream>

#include "./instructions.hpp"

InstructionSet::InstructionSet(MMU *mmu, Cpu* cpu) {
    cpu = cpu;
    mmu = mmu;
}

void InstructionSet::ldr(uint16_t reg) {
    uint8_t tmp_1 = mmu->romData[cpu->PC++]; uint8_t tmp_2 = mmu->romData[cpu->PC++];
    reg = (tmp_2 << 8) | tmp_1;
}

void InstructionSet::ldr(uint8_t reg, uint8_t address) {
    uint8_t tmp = mmu->romData[cpu->PC];
    reg = tmp;
}

void InstructionSet::ldr_mem(uint16_t reg, uint8_t address) {
    uint16_t tmp = reg;
    mmu->write8(address, tmp);
}

void InstructionSet::execute(uint8_t opcode) {
    switch (opcode) {
         case 0x00: {
            std::cout << "NOP" << std::endl;
            break;
        }
         case 0x01: {
            std::cout << "LD BC, d16" << std::endl;
            ldr(cpu->BC);
            break;
        }
         case 0x02: {
            std::cout << "LD (BC), A" << std::endl;
            ldr_mem(cpu->BC, cpu->A);
            break;
        }
         case 0x03: {
            std::cout << "INC BC" << std::endl;
            cpu->BC++;
            break;
        }
         case 0x04: {
            std::cout << "INC B" << std::endl;
            inc(cpu->B);
            break;
        }
         case 0x05: {
            std::cout << "DEC B" << std::endl;
            dec(cpu->B);
            break;
        }
         case 0x06: {
            std::cout << "LD B, d8" << std::endl;
            cpu->B = mmu->romData[cpu->PC++];
            break;
        }
         case 0x07: {
            std::cout << "RLCA" << std::endl;
            rlc(cpu->A);
            break;
        }
         case 0x08: {
            std::cout << "LD (a16), SP" << std::endl;
            uint16_t address = mmu->romData[cpu->PC + 1]
                | (mmu->romData[cpu->PC + 2] << 8);
            mmu->write16(address, cpu->SP);
            cpu->PC += 3;
            break;
        }
         case 0x09: {
            std::cout << "ADD HL, BC" << std::endl;
            add16(cpu->HL, cpu->BC);
            break;
        }
         case 0x0A: {
            std::cout << "LD A, (BC)" << std::endl;
            cpu->A = mmu->read8(cpu->BC);
            break;
        }
         case 0x0B: {
            std::cout << "DEC BC" << std::endl;
            cpu->BC--;
            break;
        }
         case 0x0C: {
            std::cout << "INC C" << std::endl;
            inc(cpu->C);
            break;
        }
         case 0x0D: {
            std::cout << "DEC C" << std::endl;
            cpu->C--;
            break;
        }
         case 0x0E: {
            std::cout << "LD C, d8" << std::endl;
            cpu->C = mmu->romData[cpu->PC++];
            break;
        }
         case 0x0F: {
            std::cout << "RRCA" << std::endl;
            rrca(cpu->A);
            break;
        }
         case 0x10: {
            break;
        }
        case 0x11: {
            std::cout << "LD DE, d16" << std::endl;
            ldr(cpu->DE);
            break;
        }
        case 0x12: {
            std::cout << "LD (DE), A" << std::endl;
            ldr_mem(cpu->DE, cpu->A);
            break;
        }
        case 0x13: {
            std::cout << "INC DE" << std::endl;
            cpu->DE++;
            break;
        }
        case 0x14: {
            std::cout << "INC D" << std::endl;
            inc(cpu->D);
            break;
        }
        case 0x15: {
            std::cout << "DEC D" << std::endl;
            dec(cpu->D);
            break;
        }
        case 0x16: {
            std::cout << "LD D, d8" << std::endl;
            cpu->D = mmu->romData[cpu->PC++];
            break;
        }
        case 0x17: {
            std::cout << "RLA" << std::endl;
            rla();
            break;
        }
        case 0x18: {
            int8_t offset;
            std::cout << "JR r8" << std::endl;
            if (!(cpu->F & FLAG_ZERO)) {
                offset = static_cast<int8_t>(mmu->romData[cpu->PC]);
            }
            cpu->PC += offset;
            cpu->PC += 1;
            break;
        }
        case 0x19: {
            std::cout << "ADD HL, DE" << std::endl;
            add16(cpu->HL, cpu->DE);
            break;
        }
        case 0x1A: {
            std::cout << "LD A, (DE)" << std::endl;
            cpu->A = mmu->read8(cpu->DE);
            break;
        }
        case 0x1B: {
            std::cout << "DEC DE" << std::endl;
            cpu->DE--;
            break;
        }
        case 0x1C: {
            std::cout << "INC E" << std::endl;
            inc(cpu->E);
            break;
        }
        case 0x1D: {
            std::cout << "DEC E" << std::endl;
            dec(cpu->E);
            break;
        }
        case 0x1E: {
            std::cout << "LD E, d8" << std::endl;
            cpu->E = mmu->romData[cpu->PC++];
            break;
        }
        case 0x1F: {
            std::cout << "RRA" << std::endl;
            rra();
            break;
        }
        case 0x20: {
            std::cout << "JR NZ, r8" << std::endl;
            if (!(cpu->F & FLAG_ZERO)) {
                int8_t offset = static_cast<int8_t>(mmu->romData[cpu->PC]);
                cpu->PC += offset;
            }
            cpu->PC += 1;
            break;
        }
        case 0x21: {
            std::cout << "LD HL, d16" << std::endl;
            ldr(cpu->HL);
            break;
        }
        case 0x22: {
            std::cout << "LD (HL+), A" << std::endl;
            ldr_mem(cpu->HL, cpu->A);
            cpu->HL++;
            break;
        }
        case 0x23: {
            std::cout << "INC HL" << std::endl;
            cpu->HL++;
            break;
        }
        case 0x24: {
            std::cout << "INC H" << std::endl;
            inc(cpu->H);
            break;
        }
        case 0x25: {
            std::cout << "DEC H" << std::endl;
            dec(cpu->H);
            break;
        }
        case 0x26: {
            std::cout << "LD H, d8" << std::endl;
            cpu->H = mmu->romData[cpu->PC++];
            break;
        }
        case 0x27: {
            std::cout << "DAA" << std::endl;
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
                if (FLAG_HALF_CARRY) correction |= 0x06;
                if (FLAG_CARRY) correction |= 0x60;
            }
            cpu->A += FLAG_SUBTRACT ? -correction : correction;
            cpu->set_flag(FLAG_ZERO, cpu->A == 0);
            cpu->set_flag(FLAG_HALF_CARRY, false);
            break;
            }
        case 0x28: {
            std::cout << "JR Z, r8" << std::endl;
            if (cpu->F & FLAG_ZERO) {
                int8_t offset = static_cast<int8_t>(mmu->romData[cpu->PC]);
                cpu->PC += offset;
            }
            cpu->PC += 1;
            break;
            }
        case 0x29: {
            std::cout << "ADD HL, HL" << std::endl;
            add16(cpu->HL, cpu->HL);
            break;
        }
        case 0x2A: {
            std::cout << "LD A, (HL+)" << std::endl;
            cpu->A = mmu->read8(cpu->HL);
            break;
        }
        case 0x2B: {
            std::cout << "DEC HL" << std::endl;
            cpu->HL--;
            break;
        }
        case 0x2C: {
            std::cout << "INC L" << std::endl;
            inc(cpu->L);
            break;
        }
        case 0x2D: {
            std::cout << "DEC L" << std::endl;
            dec(cpu->L);
            break;
        }
        case 0x2E: {
            std::cout << "LD L, d8" << std::endl;
            cpu->L = mmu->romData[cpu->PC++];
            break;
        }
        case 0x2F: {
            std::cout << "CPL" << std::endl;
            cpl(cpu->A);
            break;
        }
        case 0x30: {
            std::cout << "JR NC, r8" << std::endl;
            if (!(cpu->F & FLAG_CARRY)) {
                int8_t signed_offset =
                    static_cast<int8_t>(mmu->romData[cpu->PC++]);
                cpu->PC += signed_offset;
            }
            break;
        }
        case 0x31: {
            std::cout << "LD SP, d16" << std::endl;
            ldr(cpu->SP);
            break;
        }
        case 0x32: {
            std::cout << "LD (HL-), A" << std::endl;
            ldr_mem(cpu->HL, cpu->A);
            cpu->HL--;
            break;
        }
        case 0x33: {
            std::cout << "INC SP" << std::endl;
            cpu->SP++;
            break;
        }
        case 0x34: {
            std::cout << "INC (HL)" << std::endl;
            inc_mem(cpu->HL);
            break;
        }
        case 0x35: {
            std::cout << "DEC (HL)" << std::endl;
            dec_mem(cpu->HL);
            break;
        }
        case 0x36: {
            std::cout << "LD (HL), d8" << std::endl;
            mmu->write8(cpu->HL, mmu->romData[cpu->PC++]);
            break;
        }
        case 0x37: {
            std::cout << "SCF" << std::endl;
            cpu->set_flag(FLAG_CARRY, true);
            break;
        }
        case 0x38: {
            std::cout << "JR C, r8" << std::endl;
            int8_t offset = static_cast<int8_t>(mmu->romData[cpu->PC++]);
            if ((cpu->F & FLAG_CARRY)) {
                cpu->PC += offset;
            }
            break;
        }
        case 0x39: {
            std::cout << "ADD HL, SP" << std::endl;
            add16(cpu->HL, cpu->SP);
            break;
        }
        case 0x3A: {
            std::cout << "LD A, (HL-)" << std::endl;
            cpu->A = mmu->read8(cpu->HL);
            cpu->HL--;
            break;
        }
        case 0x3B: {
            std::cout << "DEC SP" << std::endl;
            cpu->SP--;
            break;
        }
        case 0x3C: {
            std::cout << "INC A" << std::endl;
            inc(cpu->A);
            break;
        }
        case 0x3D: {
            std::cout << "DEC A" << std::endl;
            dec(cpu->A);
            break;
        }
        case 0x3E: {
            std::cout << "LD A, d8" << std::endl;
            cpu->A = mmu->romData[cpu->PC];
            cpu->PC++;
            break;
        }
        case 0x3F: {
            std::cout << "CPL" << std::endl;
            break;
        }
        case 0x40: {
            std::cout << "LD B, B" << std::endl;
            cpu->B = cpu->B;
            break;
        }
        case 0x41: {
            std::cout << "LD B, C" << std::endl;
            cpu->B = cpu->C;
            break;
        }
        case 0x42: {
            std::cout << "LD B, D" << std::endl;
            cpu->B = cpu->D;
            break;
        }
        case 0x43: {
            std::cout << "LD B, E" << std::endl;
            cpu->B = cpu->E;
            break;
        }
        case 0x44: {
            std::cout << "LD B, H" << std::endl;
            cpu->B = cpu->H;
            break;
        }
        case 0x45: {
            std::cout << "LD B, L" << std::endl;
            cpu->B = cpu->L;
            break;
        }
        case 0x46: {
            std::cout << "LD B, (HL)" << std::endl;
            cpu->B = mmu->romData[cpu->HL];
            break;
        }
        case 0x47: {
            std::cout << "LD B, A" << std::endl;
            cpu->B = cpu->A;
            break;
        }
        case 0x48: {
            std::cout << "LD C, B" << std::endl;
            cpu->C = cpu->B;
            break;
        }
        case 0x49: {
            std::cout << "LD C, C" << std::endl;
            cpu->C = cpu->C;
            break;
        }
        case 0x4A: {
            std::cout << "LD C, D" << std::endl;
            cpu->C = cpu->D;
            break;
        }
        case 0x4B: {
            std::cout << "LD C, E" << std::endl;
            cpu->C = cpu->E;
            break;
        }
        case 0x4C: {
            std::cout << "LD C, H" << std::endl;
            cpu->C = cpu->H;
            break;
        }
        case 0x4D: {
            std::cout << "LD C, L" << std::endl;
            cpu->C = cpu->L;
            break;
        }
        case 0x4E: {
            std::cout << "LD C, (HL)" << std::endl;
            cpu->C = mmu->romData[cpu->HL];
            break;
        }
        case 0x4F: {
            std::cout << "LD C, A" << std::endl;
            cpu->C = cpu->A;
            break;
        }
        case 0x50: {
            std::cout << "LD D, B" << std::endl;
            cpu->D = cpu->B;
            break;
        }
        case 0x51: {
            std::cout << "LD D, C" << std::endl;
            cpu->D = cpu->C;
            break;
        }
        case 0x52: {
            std::cout << "LD D, D" << std::endl;
            cpu->D = cpu->D;
            break;
        }
        case 0x53: {
            std::cout << "LD D, E" << std::endl;
            cpu->D = cpu->E;
            break;
        }
        case 0x54: {
            std::cout << "LD D, H" << std::endl;
            cpu->D = cpu->H;
            break;
        }
        case 0x55: {
            std::cout << "LD D, L" << std::endl;
            cpu->D = cpu->L;
            break;
        }
        case 0x56: {
            std::cout << "LD D, (HL)" << std::endl;
            cpu->D = mmu->romData[cpu->HL];
            break;
        }
        case 0x57: {
            std::cout << "LD D, A" << std::endl;
            cpu->D = cpu->A;
            break;
        }
        case 0x58: {
            std::cout << "LD E, B" << std::endl;
            cpu->E = cpu->B;
            break;
        }
        case 0x59: {
            std::cout << "LD E, C" << std::endl;
            cpu->E = cpu->C;
            break;
        }
        case 0x5A: {
            std::cout << "LD E, D" << std::endl;
            cpu->E = cpu->D;
            break;
        }
        case 0x5B: {
            std::cout << "LD E, E" << std::endl;
            cpu->E = cpu->E;
            break;
        }
        case 0x5C: {
            std::cout << "LD E, H" << std::endl;
            cpu->E = cpu->H;
            break;
        }
        case 0x5D: {
            std::cout << "LD E, L" << std::endl;
            cpu->E = cpu->L;
            break;
        }
        case 0x5E: {
            std::cout << "LD E, (HL)" << std::endl;
            cpu->E = mmu->romData[cpu->HL];
            break;
        }
        case 0x5F: {
            std::cout << "LD E, A" << std::endl;
            cpu->E = cpu->A;
            break;
        }
        case 0x60: {
            std::cout << "LD H, B" << std::endl;
            cpu->H = cpu->B;
            break;
        }
        case 0x61: {
            std::cout << "LD H, C" << std::endl;
            cpu->H = cpu->C;
            break;
        }
        case 0x62: {
            std::cout << "LD H, D" << std::endl;
            cpu->H = cpu->D;
            break;
        }
        case 0x63: {
            std::cout << "LD H, E" << std::endl;
            cpu->H = cpu->E;
            break;
        }
        case 0x64: {
            std::cout << "LD H, H" << std::endl;
            cpu->H = cpu->H;
            break;
        }
        case 0x65: {
            std::cout << "LD H, L" << std::endl;
            cpu->H = cpu->L;
            break;
        }
        case 0x66: {
            std::cout << "LD H, (HL)" << std::endl;
            cpu->H = mmu->romData[cpu->HL];
            break;
        }
        case 0x67: {
            std::cout << "LD H, A" << std::endl;
            cpu->H = cpu->A;
            break;
        }
        case 0x68: {
            std::cout << "LD L, B" << std::endl;
            cpu->L = cpu->B;
            break;
        }
        case 0x69: {
            std::cout << "LD L, C" << std::endl;
            cpu->L = cpu->C;
            break;
        }
        case 0x6A: {
            std::cout << "LD L, D" << std::endl;
            cpu->L = cpu->D;
            break;
        }
        case 0x6B: {
            std::cout << "LD L, E" << std::endl;
            cpu->L = cpu->E;
            break;
        }
        case 0x6C: {
            std::cout << "LD L, H" << std::endl;
            cpu->L = cpu->H;
            break;
        }
        case 0x6D: {
            std::cout << "LD L, L" << std::endl;
            cpu->L = cpu->L;
            break;
        }
        case 0x6E: {
            std::cout << "LD L, (HL)" << std::endl;
            cpu->L = mmu->romData[cpu->HL];
            break;
        }
        case 0x6F: {
            std::cout << "LD L, A" << std::endl;
            cpu->L = cpu->A;
            break;
        }
        case 0x70: {
            std::cout << "LD (HL), B" << std::endl;
            mmu->romData[cpu->HL] = cpu->B;
            break;
        }
        case 0x71: {
            std::cout << "LD (HL), C" << std::endl;
            mmu->romData[cpu->HL] = cpu->C;
            break;
        }
        case 0x72: {
            std::cout << "LD (HL), D" << std::endl;
            mmu->romData[cpu->HL] = cpu->D;
            break;
        }
        case 0x73: {
            std::cout << "LD (HL), E" << std::endl;
            mmu->romData[cpu->HL] = cpu->E;
            break;
        }
        case 0x74: {
            std::cout << "LD (HL), H" << std::endl;
            mmu->romData[cpu->HL] = cpu->H;
            break;
        }
        case 0x75: {
            std::cout << "LD (HL), L" << std::endl;
            mmu->romData[cpu->HL] = cpu->L;
            break;
        }
        case 0x76: {
            break;
        }
        case 0x77: {
            std::cout << "LD (HL), A" << std::endl;
            mmu->romData[cpu->HL] = cpu->A;
            break;
        }
        case 0x78: {
            std::cout << "LD A, B" << std::endl;
            cpu->A = cpu->B;
            break;
        }
        case 0x79: {
            std::cout << "LD A, C" << std::endl;
            cpu->A = cpu->C;
            break;
        }
        case 0x7A: {
            std::cout << "LD A, D" << std::endl;
            cpu->A = cpu->D;
            break;
        }
        case 0x7B: {
            std::cout << "LD A, E" << std::endl;
            cpu->A = cpu->E;
            break;
        }
        case 0x7C: {
            std::cout << "LD A, H" << std::endl;
            cpu->A = cpu->H;
            break;
        }
        case 0x7D: {
            std::cout << "LD A, L" << std::endl;
            cpu->A = cpu->L;
            break;
        }
        case 0x7E: {
             std::cout << "LD A, (HL)" << std::endl;
             cpu->A = mmu->romData[cpu->HL];
             break;
        }
        case 0x7F: {
             std::cout << "LD A, A" << std::endl;
             cpu->A = cpu->A;
             break;
        }
        case 0x80: {
             std::cout << "ADD A, B" << std::endl;
             add8(cpu->A, cpu->B);
             break;
             case 0x81:  // ADD A, C
             std::cout << "ADD A, C" << std::endl;
             add8(cpu->A, cpu->C);
             break;
        }
        case 0x82: {
             std::cout << "ADD A, D" << std::endl;
             add8(cpu->A, cpu->D);
             break;
        }
        case 0x83: {
             std::cout << "ADD A, E" << std::endl;
             add8(cpu->A, cpu->E);
            break;
        }
        case 0x84: {
            std::cout << "ADD A, H" << std::endl;
            add8(cpu->A, cpu->H);
            break;
        }
        case 0x85: {
            std::cout << "ADD A, L" << std::endl;
            add8(cpu->A, cpu->L);
            break;
        }
        case 0x86: {
            std::cout << "ADD A, (HL)" << std::endl;
            add8_mem(cpu->A, mmu->romData[cpu->HL]);
            break;
        }
        case 0x87: {
            std::cout << "ADD A, A" << std::endl;
            add8(cpu->A, cpu->A);
            break;
        }
        case 0x88: {
            std::cout << "ADC A, B" << std::endl;
            // adc(cpu->A, cpu->B);
            break;
        }
        case 0x89: {
            std::cout << "ADC A, C" << std::endl;
            // adc(cpu->A, cpu->C);
            break;
        }
        case 0x8A: {
            std::cout << "ADC A, D" << std::endl;
            // adc(cpu->A, cpu->D);
            break;
        }
        case 0x8B: {
            std::cout << "ADC A, E" << std::endl;
            // adc(cpu->A, cpu->E);
            break;
        }
        case 0x8C: {
            std::cout << "ADC A, H" << std::endl;
            // adc(cpu->A, cpu->H);
            break;
        }
        case 0x8D: {
            std::cout << "ADC A, L" << std::endl;
            // adc(cpu->A, cpu->L);
            break;
        }
        case 0x8E: {
            std::cout << "ADC A, (HL)" << std::endl;
            // adc(cpu->A, mmu->romData[cpu->HL]);
            break;
        }
        case 0x8F: {
            std::cout << "ADC A, A" << std::endl;
            // adc(cpu->A, cpu->A);
            break;
        }
        case 0x90: {
            std::cout << "SUB B" << std::endl;
            sub(&cpu->A, &cpu->B);
            break;
        }
        case 0x91: {
            std::cout << "SUB C" << std::endl;
            sub(&cpu->A, &cpu->C);
            break;
        }
        case 0x92: {
            std::cout << "SUB D" << std::endl;
            sub(&cpu->A, &cpu->D);
            break;
        }
        case 0x93: {
            std::cout << "SUB E" << std::endl;
            sub(&cpu->A, &cpu->E);
            break;
        }
        case 0x94: {
            std::cout << "SUB H" << std::endl;
            sub(&cpu->A, &cpu->H);
            break;
        }
        case 0x95: {
            std::cout << "SUB L" << std::endl;
            sub(&cpu->A, &cpu->L);
            break;
        }
        case 0x96: {
            std::cout << "SUB (HL)" << std::endl;
            sub(&cpu->A, &mmu->romData[cpu->HL]);
            break;
        }
        case 0x97: {
            std::cout << "SUB A" << std::endl;
            sub(&cpu->A, &cpu->A);
            break;
        }
        case 0x98: {
            std::cout << "SBC A, B" << std::endl;
            // sbc(&cpu->A, &cpu->B);
            break;
        }
        case 0x99: {
            std::cout << "SBC A, C" << std::endl;
            // sbc(&cpu->A, &cpu->C);
            break;
        }
        case 0x9A: {
            std::cout << "SBC A, D" << std::endl;
            // sbc(&cpu->A, &cpu->D);
            break;
        }
        case 0x9B: {
            std::cout << "SBC A, E" << std::endl;
            // sbc(&cpu->A, &cpu->E);
            break;
        }
        case 0x9C: {
            std::cout << "SBC A, H" << std::endl;
            // sbc(&cpu->A, &cpu->H);
            break;
        }
        case 0x9D: {
            std::cout << "SBC A, L" << std::endl;
            // sbc(&cpu->A, &cpu->L);
            break;
        }
        case 0x9E: {
            std::cout << "SBC A, (HL)" << std::endl;
            // sbc(&cpu->A, &mmu->romData[cpu->HL]);
            break;
        }
        case 0x9F: {
            std::cout << "SBC A, A" << std::endl;
            // sbc(&cpu->A, &cpu->A);
            break;
        }
        case 0xA0: {
            std::cout << "AND B" << std::endl;
                // and_(&cpu->A, &cpu->B);
            break;
        }
        case 0xA1: {
            std::cout << "AND C" << std::endl;
            // and_(&cpu->A, &cpu->C);
            break;
        }
        case 0xA2: {
            std::cout << "AND D" << std::endl;
            // and_(&cpu->A, &cpu->D);
            break;
        }
        case 0xA3: {
            std::cout << "AND E" << std::endl;
            // and_(&cpu->A, &cpu->E);
            break;
        }
        case 0xA4: {
            std::cout << "AND H" << std::endl;
            // and_(&cpu->A, &cpu->H);
            break;
        }
        case 0xA5: {
            std::cout << "AND L" << std::endl;
            // and_(&cpu->A, &cpu->L);
            break;
        }
        case 0xA6: {
            std::cout << "AND (HL)" << std::endl;
            // and_(&cpu->A, &mmu->romData[cpu->HL]);
            break;
        }
        case 0xA7: {
            std::cout << "AND A" << std::endl;
            // and_(&cpu->A, &cpu->A);
            break;
        }
        case 0xA8: {
            std::cout << "XOR B" << std::endl;
            // xor_(&cpu->A, &cpu->B);
            break;
        }
        case 0xA9: {
            std::cout << "XOR C" << std::endl;
            // xor_(&cpu->A, &cpu->C);
            break;
        }
        case 0xAA: {
            std::cout << "XOR D" << std::endl;
            // xor_(&cpu->A, &cpu->D);
            break;
        }
        case 0xAB: {
            std::cout << "XOR E" << std::endl;
            // xor_(&cpu->A, &cpu->E);
            break;
        }
        case 0xAC: {
            std::cout << "XOR H" << std::endl;
            // xor_(&cpu->A, &cpu->H);
            break;
        }
        case 0xAD: {
            std::cout << "XOR L" << std::endl;
            // xor_(&cpu->A, &cpu->L);
            break;
        }
        case 0xAE: {
            std::cout << "XOR (HL)" << std::endl;
            // xor_(&cpu->A, &mmu->romData[cpu->HL]);
            break;
        }
        case 0xAF: {
            std::cout << "XOR A" << std::endl;
            // xor_(&cpu->A, &cpu->A);
            break;
        }
        case 0xB0: {
            std::cout << "OR B" << std::endl;
            or_(&cpu->A, &cpu->B);
            break;
        }
        case 0xB1: {
            std::cout << "OR C" << std::endl;
            or_(&cpu->A, &cpu->C);
            break;
        }
        case 0xB2: {
            std::cout << "OR D" << std::endl;
            or_(&cpu->A, &cpu->D);
            break;
        }
        case 0xB3: {
            std::cout << "OR E" << std::endl;
            or_(&cpu->A, &cpu->E);
            break;
        }
        case 0xB4: {
            std::cout << "OR H" << std::endl;
            or_(&cpu->A, &cpu->H);
            break;
        }
        case 0xB5: {
            std::cout << "OR L" << std::endl;
            or_(&cpu->A, &cpu->L);
            break;
        }
        case 0xB6: {
            std::cout << "OR (HL)" << std::endl;
            or_(&cpu->A, &mmu->romData[cpu->HL]);
            break;
        }
        case 0xB7: {
            std::cout << "OR A" << std::endl;
            or_(&cpu->A, &cpu->A);
            break;
        }
        case 0xB8: {
            std::cout << "CP B" << std::endl;
            cp_(&cpu->A, &cpu->B);
            break;
        }
        case 0xB9: {
            std::cout << "CP C" << std::endl;
            cp_(&cpu->A, &cpu->C);
            break;
        }
        case 0xBA: {
            std::cout << "CP D" << std::endl;
            cp_(&cpu->A, &cpu->D);
            break;
        }
        case 0xBB: {
            std::cout << "CP D" << std::endl;
            cp_(&cpu->A, &cpu->E);
            break;
        }
        case 0xBC: {
            std::cout << "CP H" << std::endl;
            cp_(&cpu->A, &cpu->H);
            break;
        }
        case 0xBD: {
            std::cout << "CP L" << std::endl;
            cp_(&cpu->A, &cpu->L);
            break;
        }
        case 0xBE: {
            std::cout << "CP (HL)" << std::endl;
            cp_(&cpu->A, &mmu->romData[cpu->HL]);
            break;
        }
        case 0xBF: {
            std::cout << "CP A" << std::endl;
            cp_(&cpu->A, &cpu->A);
            break;
        }
        case 0xC0: {
            std::cout << "RET NZ" << std::endl;
            if (!(cpu->F & FLAG_ZERO)) {
                ret(true);
            }
            break;
        }
        case 0xC1: {
            std::cout << "POP BC" << std::endl;
            cpu->C = mmu->read8(cpu->SP);
            cpu->B = mmu->read8(cpu->SP + 1);
            cpu->SP += 2;
            break;
            }
        case 0xC2: {
            std::cout << "JP NZ, nn" << std::endl;
            if (!(cpu->F & FLAG_ZERO)) {
                cpu->PC = mmu->read8(cpu->PC) | mmu->read8(cpu->PC + 1) << 8;
            } else {
                cpu->PC += 2;
            }
            break;
            }
        case 0xC3: {
            std::cout << "JP nn" << std::endl;
            cpu->PC = mmu->read8(cpu->PC) | (mmu->read8(cpu->PC + 1) << 8);
            break;
            }
        case 0xC4: {
            std::cout << "CALL nn" << std::endl;
            if (!(cpu->F & FLAG_ZERO)) {
                call(true);
            }
            break;
            }
        case 0xC5: {
            std::cout << "PUSH BC" << std::endl;
            cpu->SP -= 2;
            mmu->write8(cpu->SP + 1, cpu->B);
            mmu->write8(cpu->SP, cpu->C);
            break;
            }
        case 0xC6: {
            std::cout << "ADD A, d8" << std::endl;
            add8(cpu->A, mmu->read8(cpu->PC + 1));
            break;
            }
        case 0xC7: {
            std::cout << "RST 00h" << std::endl;
            cpu->SP -= 2;
            mmu->write8(cpu->SP, (cpu->PC >> 8) & 0xFF);
            mmu->write8(cpu->SP + 1, cpu->PC & 0xFF);
            break;
            }
        case 0xC8: {
            std::cout << "RET Z" << std::endl;
            if (cpu->F & FLAG_ZERO) {
                ret(true);
            }
            break;
            }
        case 0xC9: {
            std::cout << "RET" << std::endl;
            ret(true);
            break;
            }
        case 0xCA: {
            std::cout << "JP Z, nn" << std::endl;
            if (cpu->F & FLAG_ZERO) {
                cpu->PC =  mmu->read16(cpu->PC +1);
            }
            break;
            }
        case 0xCB: {
            std::cout << "PREFIX CB" << std::endl;
                switch (mmu->read8(cpu->PC + 1)) {
                    case 0x00:
                        rlc_extended(&cpu->B);
                        break;
                        case 0x01:  // rlc c
                            std::cout << "rlc c" << std::endl;
                             rlc_extended(&cpu->C);
                            break;
                        case 0x02:  // rlc d
                            std::cout << "rlc d" << std::endl;
                             rlc_extended(&cpu->D);
                            break;
                        case 0x03:  // RLC E
                            std::cout << "RLC E" << std::endl;
                             rlc_extended(&cpu->E);
                            break;
                        case 0x04:  // RLC H
                            std::cout << "RLC H" << std::endl;
                             rlc_extended(&cpu->H);
                            break;
                        case 0x05:  // RLC L
                            std::cout << "RLC L" << std::endl;
                             rlc_extended(&cpu->L);
                            break;
                        case 0x06:  // RLC (HL)
                            std::cout << "RLC (HL)" << std::endl;
                            // TODO(martin-montas)
                            rlc_extended_mem(&cpu->HL);
                            break;
                        case 0x07:  // RLC A
                            std::cout << "RLC A" << std::endl;
                             rlc_extended(&cpu->A);
                            break;
                        case 0x08:  // RRC B
                            std::cout << "RRC B" << std::endl;
                            rcc_extended(&cpu->B);
                            break;
                        case 0x09:  // RRC C
                            std::cout << "RRC C" << std::endl;
                            rcc_extended(&cpu->C);
                            break;
                        case 0x0A:  // RRC D
                            std::cout << "RRC D" << std::endl;
                            rcc_extended(&cpu->D);
                            break;
                        case 0x0B:  // RRC E
                            std::cout << "RRC E" << std::endl;
                            rcc_extended(&cpu->E);
                            break;
                        case 0x0C:  // RRC H
                            std::cout << "RRC H" << std::endl;
                            rcc_extended(&cpu->H);
                            break;
                        case 0x0D:  // RRC L
                            std::cout << "RRC L" << std::endl;
                            rcc_extended(&cpu->L);
                            break;
                        case 0x0E:  // RRC (HL)
                            std::cout << "RRC (HL)" << std::endl;
                            // TODO(martin-montas)
                            break;
                        case 0x0F:  // RRC A
                            std::cout << "RRC A" << std::endl;
                            rcc_extended(&cpu->A);
                            break;
                        case 0x10:  // RL B
                            std::cout << "RL B " << std::endl;
                            rl_extended(&cpu->B);
                        break;
                    case 0x11:  // RL C
                        std::cout << "RL C" << std::endl;
                        rl_extended(&cpu->A);
                        break;
                    case 0x12:  // RL D
                        std::cout << "RL D" << std::endl;
                        rl_extended(&cpu->A);
                        break;
                    case 0x13:  // RL E
                        std::cout << "RL E" << std::endl;
                        rl_extended(&cpu->A);
                        break;
                    case 0x14:  // RL H
                        std::cout << "RL H" << std::endl;
                        rl_extended(&cpu->A);
                        break;
                    case 0x15:  //  RL L
                        std::cout << "RL L " << std::endl;
                        rl_extended(&cpu->A);
                        break;
                    case 0x16:  // RL (HL)
                        std::cout << "RL (HL)" << std::endl;
                        // TODO(martin-montas)
                        break;
                    case 0x17:  // RL A
                        std::cout << "RL A" << std::endl;
                        rl_extended(&cpu->A);
                        break;
                    case 0x19:  // RR B
                        rr_extended(&cpu->B);
                        std::cout << "RR B" << std::endl;
                        break;
                    case 0x1A:  // RR C
                        rr_extended(&cpu->C);
                        std::cout << "RR C" << std::endl;
                        break;
                    case 0x1B:  // RR D
                        rr_extended(&cpu->D);
                        std::cout << "RR D" << std::endl;
                        break;
                    case 0x1C:  // RR E
                        rr_extended(&cpu->E);
                        std::cout << "RR E" << std::endl;
                        break;
                    case 0x1D:  // RR H
                        rr_extended(&cpu->H);
                        std::cout << "RR H" << std::endl;
                        break;
                    case 0x1E:  // RR L
                        rr_extended(&cpu->L);
                        std::cout << "RR L" << std::endl;
                        break;
                    case 0x1F:  // RR A
                        std::cout << "RR A" << std::endl;
                        rr_extended(&cpu->A);
                        break;
                    case 0x20:  // SLA B
                        std::cout << "SLA B" << std::endl;
                        sla_extended(&cpu->B);
                        break;
                    case 0x21:  // SLA C
                        std::cout << "SLA C" << std::endl;
                        sla_extended(&cpu->C);
                        break;
                    case 0x22:  // SLA D
                        std::cout << "SLA D" << std::endl;
                        sla_extended(&cpu->D);
                        break;
                    case 0x23:  // SLA E
                        std::cout << "SLA E" << std::endl;
                        sla_extended(&cpu->E);
                        break;
                    case 0x24:  // SLA H
                        std::cout << "SLA H" << std::endl;
                        sla_extended(&cpu->H);
                        break;
                    case 0x25:  // SLA L
                        std::cout << "SLA L" << std::endl;
                        sla_extended(&cpu->L);
                        break;
                    case 0x26:  // SLA (HL)
                        std::cout << "SLA (HL)" << std::endl;
                        // TODO(martin-montas)
                        break;
                    case 0x27:  // SLA A
                        std::cout << "SLA A" << std::endl;
                        sla_extended(&cpu->A);
                        break;
                    case 0x29:  // SRA B
                        std::cout << "SRA B" << std::endl;
                        sra_extended(&cpu->B);
                        break;
                    case 0x2A:  // SRA C
                        std::cout << "SRA C" << std::endl;
                        sra_extended(&cpu->C);
                        break;
                    case 0x2B:  // SRA D
                        std::cout << "SRA D" << std::endl;
                        sra_extended(&cpu->D);
                        break;
                    case 0x2C:  // SRA E
                        std::cout << "SRA E" << std::endl;
                        sra_extended(&cpu->E);
                        break;
                    case 0x2D:  // SRA H
                        std::cout << "SRA H" << std::endl;
                        sra_extended(&cpu->H);
                        break;
                    case 0x2E:  // SRA L
                        std::cout << "SRA L" << std::endl;
                        sra_extended(&cpu->L);
                        break;
                    case 0x2F:  // SRA A
                        std::cout << "SRA A" << std::endl;
                        sra_extended(&cpu->A);
                        break;
                    case 0x30:  // SWAP B
                        std::cout << "SWAP B" << std::endl;
                        swap_extended(&cpu->B);
                        break;
                    case 0x31:  // SWAP C
                        std::cout << "SWAP C" << std::endl;
                        swap_extended(&cpu->C);
                        break;
                    case 0x32:  // SWAP D
                        std::cout << "SWAP D" << std::endl;
                        swap_extended(&cpu->D);
                        break;
                    case 0x33:  // SWAP E
                        std::cout << "SWAP E" << std::endl;
                        swap_extended(&cpu->E);
                        break;
                    case 0x34:  // SWAP H
                        std::cout << "SWAP H" << std::endl;
                        break;
                    case 0x35:  // SWAP L
                        std::cout << "SWAP L" << std::endl;
                        swap_extended(&cpu->L);
                    case 0x36:  // SWAP (HL)
                        std::cout << "SWAP (HL)" << std::endl;
                        // TODO(martin-montas)
                        break;
                    case 0x37:  // SWAP A
                        std::cout << "SWAP A" << std::endl;
                        swap_extended(&cpu->A);
                        break;
                    case 0x39:  // SRL B
                        std::cout << "SRL B" << std::endl;
                        srl_extended(&cpu->B);
                        break;
                    case 0x3A:  // SRL C
                        std::cout << "SRL C" << std::endl;
                        srl_extended(&cpu->C);
                        break;
                    case 0x3B:  // SRL D
                        std::cout << "SRL D" << std::endl;
                        srl_extended(&cpu->D);
                        break;
                    case 0x3C:  // SRL E
                        std::cout << "SRL E" << std::endl;
                        srl_extended(&cpu->E);
                        break;
                    case 0x3D:  // SRL H
                        std::cout << "SRL H" << std::endl;
                        srl_extended(&cpu->H);
                        break;
                    case 0x3E:  // SRL L
                        std::cout << "SRL L " << std::endl;
                        srl_extended(&cpu->L);
                        break;
                    case 0x3F:  // SRL A
                        std::cout << "SRL A" << std::endl;
                        srl_extended(&cpu->A);
                        break;
                    default:
                        std::cerr << "Unknown opcode: 0x"
                        << std::hex << static_cast<int>(mmu->read8(cpu->PC + 1))
                        << std::endl;
                        break;
                }
                // Breaks out of the switch block:
                break;
            }

        case 0xCC: {
            std::cout << "CALL nn" << std::endl;
            if (cpu->F & FLAG_ZERO) {
                execute_call();
            }else {
                cpu->PC += 2;
            }
            break;
            }
        case 0xCD: {
            std::cout << "CALL a16" << std::endl;
            uint16_t address =  mmu->read16(mmu->romData[cpu->PC +1]);
            cpu->PC = address;
            break;
            }
        case 0xCE: {
            std::cout <<  "ADC nn" << std::endl;
            break;
            }
        case 0xCF: {
            std::cout << "RST 08H" << std::endl;
            break;
            }
        case 0xD0: {
            std::cout << " RET NC" << std::endl;
            break;
        }
        case 0xD1: {
            std::cout << " POP DE" << std::endl;
            break;
        }
        case 0xD2: {
            std::cout << "  JPNC, nn" << std::endl;
            break;
        }
        case 0xD3: {
            std::cout << "  OUT(n), A" << std::endl;
            break;
        }
        case 0xD4: {
            std::cout << "  CALLNC, nn" << std::endl;
            break;
        }
        case 0xD5: {
            std::cout << " PUSH DE" << std::endl;
            break;
        }
        case 0xD6: {
            std::cout << " SUB nn" << std::endl;
            break;
        }
        case 0xD7: {
            std::cout << " RST 10H" << std::endl;
            break;
        }
        case 0xD8: {
            std::cout << " RET C" << std::endl;
            break;
        }
        case 0xD9: {
            std::cout << " RETI" << std::endl;
            break;
        }
        case 0xDA: {
            std::cout << "  JPC, nn" << std::endl;
            break;
        }
        case 0xDB: {
            std::cout << "   INA, (n)" << std::endl;
            break;
        }
        case 0xDD: {
            std::cout << "  CALLC, nn" << std::endl;
            break;
        }
        case 0xDE: {
            std::cout << " SBC nn" << std::endl;
            break;
        }
        case 0xDF: {
            std::cout << " RST 18H" << std::endl;
            break;
        }
        case 0xE0: {
            std::cout << "  LDH(n), A" << std::endl;
            break;
        }
        case 0xE1: {
            std::cout << " POP HL" << std::endl;
            break;
        }
        case 0xE2: {
            std::cout << "  LD(C), A" << std::endl;
            break;
        }
        case 0xE3: {
            std::cout << " XOR nn" << std::endl;
            break;
        }
        case 0xE4: {
            std::cout << " PUSH HL" << std::endl;
            break;
        }
        case 0xE5: {
            std::cout << " AND nn" << std::endl;
            break;
        }
        case 0xE6: {
            std::cout << " XOR nn" << std::endl;
            break;
        }
        case 0xE7: {
            std::cout << " RST 20H" << std::endl;
            break;
        }
        case 0xE8: {
            std::cout << "  ADDSP, r8" << std::endl;
            break;
        }
        case 0xE9: {
            std::cout << "  JP(HL)" << std::endl;
            break;
        }
        case 0xEA: {
            std::cout << "  LD(nn), A" << std::endl;
            break;
        }
        case 0xEB: {
            std::cout << " XOR nn" << std::endl;
            break;
        }
        case 0xED: {
            std::cout << " CALL nn" << std::endl;
            break;
        }
        case 0xEE: {
            std::cout << " XOR nn" << std::endl;
            break;
        }
        case 0xEF: {
            std::cout << " RST 28H" << std::endl;
            break;
        }
        case 0xF0: {
            std::cout << "  LDHA, (n)" << std::endl;
            break;
        }
        case 0xF1: {
            std::cout << " POP AF" << std::endl;
            break;
        }
        case 0xF2: {
            std::cout << "  LDA, (C)" << std::endl;
            break;
        }
        case 0xF3: {
            std::cout << " DI" << std::endl;
            break;
        }
        case 0xF4: {
            std::cout << " PUSH AF" << std::endl;
            break;
        }
        case 0xF5: {
            std::cout << " OR nn" << std::endl;
            break;
        }
        case 0xF6: {
            std::cout << " OR nn" << std::endl;
            break;
        }
        case 0xF7: {
            std::cout << " RST 30H" << std::endl;
            break;
        }
        case 0xF8: {
            std::cout << "  LD HL, SP+ r8" << std::endl;
            break;
        }
        case 0xF9: {
            std::cout << "  LDSP, HL" << std::endl;
            break;
        }
        case 0xFA: {
            std::cout << "  LD A, (nn)" << std::endl;
            break;
        }
        case 0xFB: {
            std::cout << " EI" << std::endl;
            break;
        }
        case 0xFD: {
            std::cout << " CALL nn" << std::endl;
            break;
        }
        case 0xFE: {
            std::cout << " CP nn" << std::endl;
            break;
        }
        case 0xFF: {
            std::cout << " RST 38H" << std::endl;
            break;
        }
        default: {
            std::cerr << "Unknown opcode: 0x"
            << std::hex << static_cast<int>(mmu->read8(cpu->PC + 1))
            << std::endl;
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

void inc_mem(uint8_t value) {
    return;
}
void InstructionSet::cpl(uint8_t reg) {
    reg = ~reg;

    cpu->set_flag(FLAG_SUBTRACT, true);
    cpu->set_flag(FLAG_HALF_CARRY, true);
}

void InstructionSet::inc_mem(uint16_t reg) {
    uint8_t tmp = mmu->read8(reg);
    uint8_t nibble_carry = tmp & 0x0F;
    tmp++;
    mmu->write8(reg, tmp);

    cpu->set_flag(FLAG_HALF_CARRY, (nibble_carry == 0x0F));
    cpu->set_flag(FLAG_ZERO, (reg == 0));
    cpu->clear_flag(FLAG_SUBTRACT);
}

void InstructionSet::inc(uint8_t reg) {
    uint8_t nibble_carry = reg & 0x0F;

    (reg)++;
    cpu->set_flag(FLAG_HALF_CARRY, (nibble_carry == 0x0F));

    cpu->set_flag(FLAG_ZERO, (reg == 0));
    cpu->clear_flag(FLAG_SUBTRACT);
}

void InstructionSet::inc(uint16_t reg) {
    uint8_t nibble_carry = reg & 0x0F;

    (reg)++;
    cpu->set_flag(FLAG_HALF_CARRY, (nibble_carry == 0x0F));

    cpu->set_flag(FLAG_ZERO, (reg == 0));
    cpu->clear_flag(FLAG_SUBTRACT);
}

void InstructionSet::dec(uint8_t reg) {
    cpu->set_flag(FLAG_HALF_CARRY, (reg & 0x0F) == 0);
    (reg)--;

    cpu->set_flag(FLAG_ZERO, (reg == 0));
    cpu->set_flag(FLAG_SUBTRACT, true);
}

void InstructionSet::add8(uint8_t reg_1, uint8_t reg_2) {
    uint8_t tmp = reg_1 + reg_2;
    cpu->set_flag(FLAG_CARRY, tmp > 0xFF);
    cpu->set_flag(FLAG_ZERO, (tmp == 0));
    cpu->set_flag(FLAG_HALF_CARRY, ((reg_1 & 0x0F) + (reg_2 & 0x0F)) > 0x0F);
    cpu->clear_flag(FLAG_SUBTRACT);
    reg_1 = tmp & 0xFF;
}

void InstructionSet::add16(uint16_t destination, uint16_t value) {
    uint32_t result = destination + value;
    cpu->F &= ~FLAG_SUBTRACT;
    cpu->set_flag(FLAG_CARRY, result > 0xFFFF);

    cpu->set_flag(FLAG_HALF_CARRY, ((destination & 0x0FFF)
                + (value & 0x0FFF)) > 0x0FFF);
    destination = result & 0xFFFF;
}

void ldhl(int8_t value) {
}


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
    uint16_t result = *reg_1 - *reg_2;

    *reg_1 = result & 0xFF;

    cpu->set_flag(FLAG_ZERO, (*reg_1 == 0));
    cpu->set_flag(FLAG_SUBTRACT, 1);
    cpu->set_flag(FLAG_HALF_CARRY, ((*reg_1 & 0x0F) < (result & 0x0F)));
    cpu->set_flag(FLAG_CARRY, (result > 0xFF));
}

void InstructionSet::rlc(uint8_t reg) {
    bool msb = reg & 0x80;
    reg = (reg << 1) | (msb >> 7);

    cpu->set_flag(FLAG_CARRY, msb);

    cpu->set_flag(FLAG_ZERO, reg == 0);

    cpu->clear_flag(FLAG_SUBTRACT);
    cpu->clear_flag(FLAG_HALF_CARRY);
}

void InstructionSet::rrca(uint8_t reg) {
    bool least_sig_bit = reg & 1;
    cpu->A = reg >> 1;
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
    uint8_t  tmp = mmu->read8(reg);
    uint8_t nibble_carry = tmp & 0x0F;
    tmp--;
    mmu->write8(reg, tmp);

    cpu->set_flag(FLAG_HALF_CARRY, nibble_carry == 0);
    cpu->set_flag(FLAG_ZERO, nibble_carry == 0);
    cpu->set_flag(FLAG_SUBTRACT, true);
}

void InstructionSet::rla() {
    std::cout << "RLA" << std::endl;
    bool carry = cpu->F & FLAG_CARRY;
    cpu->set_flag(FLAG_CARRY, cpu->A & 0x80);
    cpu->A = cpu->A << 1;
    if (carry) {
        cpu->A |= 0x01;
    }
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
void dec_mem(uint8_t *value) {
    return;
}

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
