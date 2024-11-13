#include "instructions.hpp"
#include "cpu/CPU.hpp"
#include <cstdint>
#include <iostream>


InstructionSet::InstructionSet(CPU &cpu, MMU &mmu) : cpu(cpu), mmu(mmu){}

void InstructionSet::ldr(uint16_t *reg) {
    uint8_t tmp_1 = mmu.romData[cpu.PC++];
    uint8_t tmp_2 = mmu.romData[cpu.PC++];
    *reg = (tmp_2 << 8) | tmp_1;
}

void InstructionSet::ldr(uint8_t *reg, uint8_t *address) {
    uint8_t tmp = mmu.romData[cpu.PC];
    *reg = tmp;
}

void InstructionSet::ldr_mem(uint16_t *reg, uint8_t *address) {
    uint16_t tmp = *reg;
    mmu.write8(*address, tmp); 
}

void InstructionSet::execute(uint8_t opcode) {
    switch (opcode) {
        case 0x00:  // NOP
            std::cout << "NOP" << std::endl;
            break;

        case 0x01:  // LD BC, d16
            std::cout << "LD BC, d16" << std::endl;
            ldr(&cpu.BC);
            break;

        case 0x02:  // LD (BC), A 
            std::cout << "LD (BC), A" << std::endl;
            ldr_mem(&cpu.BC, &cpu.A);
            break;

        case 0x03: // INC BC 
            std::cout << "INC BC" << std::endl;
            cpu.BC++; 
            break;

        case 0x04: // INC B 
            std::cout << "INC B" << std::endl;
            inc(&cpu.B);
            break;

        case 0x05: // DEC B  
            std::cout << "DEC B" << std::endl;
            dec(&cpu.B);
            break;

        case 0x06: // LD B, d8 
            std::cout << "LD B, d8" << std::endl;
            cpu.B = mmu.romData[cpu.PC++];
            break;

        case 0x07: // RLCA 
            std::cout << "RLCA" << std::endl;
            rlc(&cpu.A);
            break;

        case 0x08: // LD (a16), SP 
            std::cout << "LD (a16), SP" << std::endl;
            uint16_t address = mmu.romData[cpu.PC + 1] | (mmu.romData[cpu.PC + 2] << 8);
            mmu.write16(address, cpu.SP);
            cpu.PC += 3;
            break;

        case 0x09:  // ADD HL, BC
            std::cout << "ADD HL, BC" << std::endl;
            add16(&cpu.HL, &cpu.BC);
            break;

        case 0x0A:  // LD A, (BC)
            std::cout << "LD A, (BC)" << std::endl;
            cpu.A = mmu.read8(cpu.BC);
            break;

        case 0x0B:  // DEC BC
            std::cout << "DEC BC" << std::endl;
            cpu.BC--;
            break;

        case 0x0C: // INC C 
            std::cout << "INC C" << std::endl;
            inc(&cpu.C);
            break;

        case 0x0D:  // DEC C
            std::cout << "DEC C" << std::endl;
            cpu.C--;
            break;

        case 0x0E: // LD C, d8
            std::cout << "LD C, d8" << std::endl;
            cpu.C = mmu.romData[cpu.PC++];
            break;

        case 0x0F: // RRCA
            std::cout << "RRCA" << std::endl;
            rrca(&cpu.A);
            break;

        case 0x10:  // STOP
            break;

        case 0x11: // LD DE, d16
            std::cout << "LD DE, d16" << std::endl;
            ldr(&cpu.DE);
            break;

        case 0x12: // LD (DE), A
            std::cout << "LD (DE), A" << std::endl;
            ldr_mem(&cpu.DE, &cpu.A);
            break;

        case 0x13: // INC DE
            std::cout << "INC DE" << std::endl;
            cpu.DE++;
            break;

        case 0x14: // INC D
            std::cout << "INC D" << std::endl;
            inc(&cpu.D);
            break;

        case 0x15: // DEC D
            std::cout << "DEC D" << std::endl;
            dec(&cpu.D);
            break;

        case 0x16: // LD D, d8
            std::cout << "LD D, d8" << std::endl;
            cpu.D = mmu.romData[cpu.PC++];
            break;

        case 0x17: // RLA 
            std::cout << "RLA" << std::endl;
            rla();
            break;

        case 0x18:  // JR r8
            std::cout << "JR r8" << std::endl;
            if (!(cpu.F & FLAG_ZERO)) {
                int8_t offset = static_cast<int8_t>(mmu.romData[cpu.PC]); 
                cpu.PC += offset; 
            }
            cpu.PC += 1; 
            break;

        case 0x19:  // ADD HL, DE
            std::cout << "ADD HL, DE" << std::endl;
            add16(&cpu.HL, &cpu.DE);
            break;

        case 0x1A:  // LD A, (DE)
            std::cout << "LD A, (DE)" << std::endl;
            cpu.A = mmu.read8(cpu.DE);
            break;

        case 0x1B: // DEC DE
            std::cout << "DEC DE" << std::endl;
            cpu.DE--;
            break;

        case 0x1C: // INC E
            std::cout << "INC E" << std::endl;
            inc(&cpu.E);
            break;

        case 0x1D: // DEC E
            std::cout << "DEC E" << std::endl;
            dec(&cpu.E);
            break;

        case 0x1E: // LD E, d8 
            std::cout << "LD E, d8" << std::endl;
            cpu.E = mmu.romData[cpu.PC++];
            break;

        case 0x1F: // RRA
            std::cout << "RRA" << std::endl;
            rra();
            break;

        case 0x20:  // JR NZ, r8
            std::cout << "JR NZ, r8" << std::endl;
            if (!(cpu.F & FLAG_ZERO)) {
                int8_t offset = static_cast<int8_t>(mmu.romData[cpu.PC]); 
                cpu.PC += offset; 
            }
            cpu.PC += 1; 
            break;

        case 0x21: // LD HL, d16
            std::cout << "LD HL, d16" << std::endl;
            ldr(&cpu.HL);
            break;

        case 0x22:  // LD (HL+), A
            std::cout << "LD (HL+), A" << std::endl;
            ldr_mem(&cpu.HL, &cpu.A);
            cpu.HL++;
            break;

        case 0x23: // INC HL
            std::cout << "INC HL" << std::endl;
            cpu.HL++;
            break;

        case 0x24: // INC H
            std::cout << "INC H" << std::endl;
            inc(&cpu.H);
            break;

        case 0x25: // DEC H 
            std::cout << "DEC H" << std::endl;
            dec(&cpu.H);
            break;

        case 0x26:  // LD H, d8
            std::cout << "LD H, d8" << std::endl;
            cpu.H = mmu.romData[cpu.PC++];
            break;

        case 0x27:  // DAA
            uint8_t correction = 0;
            if (!(cpu.F & FLAG_SUBTRACT)) {

                if ((cpu.A & 0x0F) > 9 || FLAG_HALF_CARRY) {
                    correction |= 0x06; 
                }

                if ((cpu.A > 0x99) || FLAG_HALF_CARRY) {
                    correction |= 0x60;
                    cpu.set_flag(FLAG_HALF_CARRY, true);
                }
            } else {
                if (FLAG_HALF_CARRY) correction |= 0x06;
                if (FLAG_CARRY) correction |= 0x60;
            }


            cpu.A += FLAG_SUBTRACT ? -correction : correction;
            cpu.set_flag(FLAG_ZERO, cpu.A == 0);
            cpu.set_flag(FLAG_HALF_CARRY, false);
            break;

        case 0x28:  // JR Z, r8
            std::cout << "JR Z, r8" << std::endl;
            if (cpu.F & FLAG_ZERO) {
                int8_t offset = static_cast<int8_t>(mmu.romData[cpu.PC]); 
                cpu.PC += offset; 
            }
            cpu.PC += 1; 
            break;

        case 0x29: // ADD HL, HL
            std::cout << "ADD HL, HL" << std::endl;
            add16(&cpu.HL, &cpu.HL);
            break;

        case 0x2A: // LD A, (HL+)
            std::cout << "LD A, (HL+)" << std::endl;
            cpu.A = mmu.read8(cpu.HL);
            break;

        case 0x2B: // DEC HL
            std::cout << "DEC HL" << std::endl;
            cpu.HL--;
            break;

        case 0x2C: // INC L
            std::cout << "INC L" << std::endl;
            inc(&cpu.L);
            break;

        case 0x2D: // DEC L
            std::cout << "DEC L" << std::endl;
            dec(&cpu.L);
            break;

        case 0x2E: // LD L, d8
            std::cout << "LD L, d8" << std::endl;
            cpu.L = mmu.romData[cpu.PC++];
            break;

        case 0x2F: // CPL
            std::cout << "CPL" << std::endl;
            cpl(&cpu.A);
            break;

        case 0x30: // JR NC, r8
            std::cout << "JR NC, r8" << std::endl;
            if (!(cpu.F & FLAG_CARRY)) {
                int8_t signed_offset = static_cast<int8_t>(mmu.romData[cpu.PC++]);
                cpu.PC += signed_offset;
            }
            break;

        case 0x31: // LD SP, d16
            std::cout << "LD SP, d16" << std::endl;
            ldr(&cpu.SP);
            break;

        case 0x32: // LD (HL-), A
            std::cout << "LD (HL-), A" << std::endl;
            ldr_mem(&cpu.HL, &cpu.A);
            cpu.HL--;
            break;

        case 0x33:  // INC SP
            std::cout << "INC SP" << std::endl;
            cpu.SP++;
            break;

        case 0x34: // INC (HL)
            std::cout << "INC (HL)" << std::endl;
            inc_mem(&cpu.HL);
            break;

        case 0x35: // DEC (HL)
            std::cout << "DEC (HL)" << std::endl;
            dec_mem(&cpu.HL);
            break;

        case 0x36: // LD (HL), d8
            std::cout << "LD (HL), d8" << std::endl;
            mmu.write8(cpu.HL, mmu.romData[cpu.PC++]);
            break;

        case 0x37: // SCF
            std::cout << "SCF" << std::endl;
            cpu.set_flag(FLAG_CARRY, true);
            break;

        case 0x38: // JR C, r8
            std::cout << "JR C, r8" << std::endl;
            int8_t offset = static_cast<int8_t>(mmu.romData[cpu.PC++]);

            if ((cpu.F & FLAG_CARRY)) {
                cpu.PC += offset;
            }
            break;

        case 0x39: // ADD HL, SP
            std::cout << "ADD HL, SP" << std::endl;
            add16(&cpu.HL, &cpu.SP);
            break;

        case 0x3A: // LD A, (HL-)
            std::cout << "LD A, (HL-)" << std::endl;
            cpu.A = mmu.read8(cpu.HL);
            cpu.HL--;
            break;

        case 0x3B: // DEC SP
            std::cout << "DEC SP" << std::endl;
            cpu.SP--;
            break;

        case 0x3C: // INC A
            std::cout << "INC A" << std::endl;
            inc(&cpu.A);
            break;

        case 0x3D:  // DEC A
            std::cout << "DEC A" << std::endl;
            dec(&cpu.A);
            break;

        case 0x3E:  // LD A, d8
            std::cout << "LD A, d8" << std::endl;
            cpu.A = mmu.romData[cpu.PC];
            cpu.PC++;
            break;

        case 0x3F:  // CPL
            std::cout << "CPL" << std::endl;
            break;

        case 0x40: // LD B, B
            std::cout << "LD B, B" << std::endl;
            cpu.B = cpu.B;
            break;

        case 0x41:  // LD B, C
            std::cout << "LD B, C" << std::endl;
            cpu.B = cpu.C;
            break;

        case 0x42:  // LD B, D
            std::cout << "LD B, D" << std::endl;
            cpu.B = cpu.D;
            break;

        case 0x43:  // LD B, E
            std::cout << "LD B, E" << std::endl;
            cpu.B = cpu.E;
            break;

        case 0x44:  // LD B, H
            std::cout << "LD B, H" << std::endl;
            cpu.B = cpu.H;
            break;

        case 0x45: // LD B, L
            std::cout << "LD B, L" << std::endl;
            cpu.B = cpu.L;
            break;

        case 0x46: // LD B, (HL)
            std::cout << "LD B, (HL)" << std::endl;
            cpu.B = mmu.romData[cpu.HL];
            break;

        case 0x47:  // LD B, A
            std::cout << "LD B, A" << std::endl;
            cpu.B = cpu.A;
            break;

        case 0x48:  // LD C, B
            std::cout << "LD C, B" << std::endl; 
            cpu.C = cpu.B;
            break;

        case 0x49: // LD C, C
            std::cout << "LD C, C" << std::endl;
            cpu.C = cpu.C;
            break;

        case 0x4A: // LD C, D
            std::cout << "LD C, D" << std::endl;
            cpu.C = cpu.D;
            break;

        case 0x4B: // LD C, E
            std::cout << "LD C, E" << std::endl;
            cpu.C = cpu.E;
            break;

        case 0x4C: // LD C, H
            std::cout << "LD C, H" << std::endl;
            cpu.C = cpu.H;
            break;

        case 0x4D:  // LD C, L
            std::cout << "LD C, L" << std::endl;
            cpu.C = cpu.L;
            break;

        case 0x4E: // LD C, (HL)
            std::cout << "LD C, (HL)" << std::endl;
            cpu.C = mmu.romData[cpu.HL];
            break;

        case 0x4F: // LD C, A
            std::cout << "LD C, A" << std::endl;
            cpu.C = cpu.A;
            break;

        case 0x50: // LD D, B
            std::cout << "LD D, B" << std::endl;
            cpu.D = cpu.B;
            break;

        case 0x51: // LD D, C
            std::cout << "LD D, C" << std::endl;
            cpu.D = cpu.C;
            break;

        case 0x52:  // LD D, D
            std::cout << "LD D, D" << std::endl;
            cpu.D = cpu.D;
            break;

        case 0x53:  // LD D, E
            std::cout << "LD D, E" << std::endl;
            cpu.D = cpu.E;
            break;

        case 0x54:  // LD D, H
            std::cout << "LD D, H" << std::endl;
            cpu.D = cpu.H;
            break;

        case 0x55: // LD D, L
            std::cout << "LD D, L" << std::endl;
            cpu.D = cpu.L;
            break;

        case 0x56:  // LD D, (HL)
            std::cout << "LD D, (HL)" << std::endl;
            cpu.D = mmu.romData[cpu.HL];
            break;

        case 0x57: // LD D, A
            std::cout << "LD D, A" << std::endl;
            cpu.D = cpu.A;
            break;

        case 0x58: // LD E, B
            std::cout << "LD E, B" << std::endl;
            cpu.E = cpu.B;
            break;

        case 0x59: // LD E, C
            std::cout << "LD E, C" << std::endl;
            cpu.E = cpu.C;
            break;

        case 0x5A: // LD E, D
            std::cout << "LD E, D" << std::endl;
            cpu.E = cpu.D;
            break;

        case 0x5B: // LD E, E
            std::cout << "LD E, E" << std::endl;
            cpu.E = cpu.E;
            break;

        case 0x5C: // LD E, H
            std::cout << "LD E, H" << std::endl;
            cpu.E = cpu.H;
            break;

        case 0x5D: // LD E, L
            std::cout << "LD E, L" << std::endl;
            cpu.E = cpu.L;
            break;

        case 0x5E: // LD E, (HL)
            std::cout << "LD E, (HL)" << std::endl;
            cpu.E = mmu.romData[cpu.HL];
            break;

        case 0x5F: // LD E, A
            std::cout << "LD E, A" << std::endl;
            cpu.E = cpu.A;
            break;

        case 0x60: // LD H, B
            std::cout << "LD H, B" << std::endl;
            cpu.H = cpu.B;
            break;

        case 0x61: // LD H, C
            std::cout << "LD H, C" << std::endl;
            cpu.H = cpu.C;
            break;

        case 0x62:  // LD H, D
            std::cout << "LD H, D" << std::endl;
            cpu.H = cpu.D;
            break;

        case 0x63:  // LD H, E
            std::cout << "LD H, E" << std::endl;
            cpu.H = cpu.E;
            break;

        case 0x64: // LD H, H
            std::cout << "LD H, H" << std::endl;
            cpu.H = cpu.H;
            break;

        case 0x65: // LD H, L
            std::cout << "LD H, L" << std::endl;
            cpu.H = cpu.L;
            break;

        case 0x66: // LD H, (HL)
            std::cout << "LD H, (HL)" << std::endl;
            cpu.H = mmu.romData[cpu.HL];
            break;

        case 0x67:  // LD H, A
            std::cout << "LD H, A" << std::endl;
            cpu.H = cpu.A;
            break;

        case 0x68:  // LD L, B
            std::cout << "LD L, B" << std::endl;
            cpu.L = cpu.B;
            break;

        case 0x69: // LD L, C
            std::cout << "LD L, C" << std::endl;
            cpu.L = cpu.C;
            break;

        case 0x6A: // LD L, D
            std::cout << "LD L, D" << std::endl;
            cpu.L = cpu.D;
            break;

        case 0x6B: // LD L, E
            std::cout << "LD L, E" << std::endl;
            cpu.L = cpu.E;
            break;

        case 0x6C:  // LD L, H
            std::cout << "LD L, H" << std::endl;
            cpu.L = cpu.H;
            break;

        case 0x6D: // LD L, L
            std::cout << "LD L, L" << std::endl;
            cpu.L = cpu.L;
            break;

        case 0x6E: // LD L, (HL)
            std::cout << "LD L, (HL)" << std::endl;
            cpu.L = mmu.romData[cpu.HL];
            break;

        case 0x6F: // LD L, A
            std::cout << "LD L, A" << std::endl;
            cpu.L = cpu.A;
            break;

        case 0x70: // LD (HL), B
            std::cout << "LD (HL), B" << std::endl;
            mmu.romData[cpu.HL] = cpu.B;
            break;

        case 0x71:  // LD (HL), C
            std::cout << "LD (HL), C" << std::endl;
            mmu.romData[cpu.HL] = cpu.C; 
            break;

        case 0x72:  // LD (HL), D
            std::cout << "LD (HL), D" << std::endl;
            mmu.romData[cpu.HL] = cpu.D;
            break;

        case 0x73:  // LD (HL), E
            std::cout << "LD (HL), E" << std::endl;
            mmu.romData[cpu.HL] = cpu.E; 
            break;

        case 0x74:  // LD (HL), H
            std::cout << "LD (HL), H" << std::endl;
            mmu.romData[cpu.HL] = cpu.H;
            break;

        case 0x75:  // LD (HL), L
            std::cout << "LD (HL), L" << std::endl;
            mmu.romData[cpu.HL] = cpu.L;
            break;

        case 0x76:  // HALT
            break;

        case 0x77:  // LD (HL), A
            std::cout << "LD (HL), A" << std::endl;
            mmu.romData[cpu.HL] = cpu.A;
            break;

        case 0x78:  // LD A, B
            std::cout << "LD A, B" << std::endl;
            cpu.A = cpu.B;
            break;

        case 0x79: // LD A, C
            std::cout << "LD A, C" << std::endl;
            cpu.A = cpu.C;
            break;

        case 0x7A: // LD A, D
            std::cout << "LD A, D" << std::endl;
            cpu.A = cpu.D;
            break;

        case 0x7B: // LD A, E
            std::cout << "LD A, E" << std::endl;
            cpu.A = cpu.E;
            break;

        case 0x7C: // LD A, H
            std::cout << "LD A, H" << std::endl;
            cpu.A = cpu.H;
            break;

        case 0x7D: // LD A, L
            std::cout << "LD A, L" << std::endl;
            cpu.A = cpu.L;
            break;

        case 0x7E: // LD A, (HL)
            std::cout << "LD A, (HL)" << std::endl;
            cpu.A = mmu.romData[cpu.HL];
            break;

        case 0x7F:  // LD A, A
            std::cout << "LD A, A" << std::endl;
            cpu.A = cpu.A;
            break;

        case 0x80: // ADD A, B
            std::cout << "ADD A, B" << std::endl;
            add8(&cpu.A, cpu.B);
            break;
        case 0x81:  // ADD A, C
            std::cout << "ADD A, C" << std::endl;
            add8(&cpu.A, cpu.C);
            break;

        case 0x82:  // ADD A, D
            std::cout << "ADD A, D" << std::endl;
            add8(&cpu.A, cpu.D);
            break;

        case 0x83:  // ADD A, E
            std::cout << "ADD A, E" << std::endl; 
            add8(&cpu.A, cpu.E);
            break;

        case 0x84:  // ADD A, H
            std::cout << "ADD A, H" << std::endl;
            add8(&cpu.A, cpu.H);
            break;

        case 0x85:  // ADD A, L
            std::cout << "ADD A, L" << std::endl;
            add8(&cpu.A, cpu.L);
            break;

        case 0x86:  // ADD A, (HL)
            std::cout << "ADD A, (HL)" << std::endl;
            add8_mem(&cpu.A, mmu.romData[cpu.HL]);
            break;

        case 0x87:  // ADD A, A
            std::cout << "ADD A, A" << std::endl;
            add8(&cpu.A, cpu.A);
            break;

        case 0x88:  // ADC A, B
            std::cout << "ADC A, B" << std::endl;
            adc(cpu.A,cpu.B);
            break;

        case 0x89: // ADC A, C
            std::cout << "ADC A, C" << std::endl;
            adc(cpu.A,cpu.C);
            break;

        case 0x8A: // ADC A, D
            std::cout << "ADC A, D" << std::endl;
            adc(cpu.A,cpu.D);
            break;

        case 0x8B:  // ADC A, E
            std::cout << "ADC A, E" << std::endl;
            adc(cpu.A,cpu.E);
            break;

        case 0x8C: // ADC A, H
            std::cout << "ADC A, H" << std::endl;
            adc(cpu.A,cpu.H);
            break;

        case 0x8D:  // ADC A, L
            std::cout << "ADC A, L" << std::endl;
            adc(cpu.A,cpu.L);
            break;

        case 0x8E:  // ADC A, (HL)
            std::cout << "ADC A, (HL)" << std::endl;
            adc(cpu.A,mmu.romData[cpu.HL]);
            break;

        case 0x8F:  // ADC A, A
            std::cout << "ADC A, A" << std::endl;
            adc(cpu.A,cpu.A);
            break;

        case 0x90:  // SUB B
            std::cout << "SUB B" << std::endl;
            sub(cpu.A,cpu.B);
            break;

        case 0x91:  // SUB C
            std::cout << "SUB C" << std::endl;
            sub(cpu.A,cpu.C);
            break;

        case 0x92:  // SUB D
            std::cout << "SUB D" << std::endl;
            sub(cpu.A,cpu.D);
            break;

        case 0x93:  // SUB E
            std::cout << "SUB E" << std::endl;
            sub(cpu.A,cpu.E);
            break;

        case 0x94:  // SUB H
            std::cout << "SUB H" << std::endl;
            sub(cpu.A,cpu.H);
            break;

        case 0x95:  // SUB L
            std::cout << "SUB L" << std::endl;
            sub(cpu.A,cpu.L);
            break;

        case 0x96:  // SUB (HL)
            std::cout << "SUB (HL)" << std::endl;
            sub(cpu.A,mmu.romData[cpu.HL]);
            break;

        case 0x97:  // SUB A
            std::cout << "SUB A" << std::endl;
            sub(cpu.A,cpu.A);
            break;

        case 0x98:  // SBC A, B
            std::cout << "SBC A, B" << std::endl;
            sbc(cpu.A,cpu.B);
            break;

        case 0x99: // SBC A, C
            std::cout << "SBC A, C" << std::endl;
            sbc(cpu.A,cpu.C);
            break;

        case 0x9A: // SBC A, D
            std::cout << "SBC A, D" << std::endl;
            sbc(cpu.A,cpu.D);
            break;

        case 0x9B:  // SBC A, E
            std::cout << "SBC A, E" << std::endl;
            sbc(cpu.A,cpu.E);
            break;

        case 0x9C: // SBC A, H
            std::cout << "SBC A, H" << std::endl;
            sbc(cpu.A,cpu.H); break;

        case 0x9D:  // SBC A, L
            std::cout << "SBC A, L" << std::endl;
            sbc(cpu.A,cpu.L);
            break;

        case 0x9E: // SBC A, (HL)
            std::cout << "SBC A, (HL)" << std::endl;
            sbc(cpu.A,mmu.romData[cpu.HL]);
            break;

        case 0x9F:  // SBC A, A
            std::cout << "SBC A, A" << std::endl;
            sbc(cpu.A,cpu.A);
            break;

        case 0xA0:  // AND B
            std::cout << "AND B" << std::endl;
            and_(cpu.A,cpu.B);
            break;

        case 0xA1:  // AND C
            std::cout << "AND C" << std::endl;
            and_(cpu.A,cpu.C);
            break;

        case 0xA2: // AND D
            std::cout << "AND D" << std::endl;
            and_(cpu.A,cpu.D);
            break;

        case 0xA3:  // AND E
            std::cout << "AND E" << std::endl;
            and_(cpu.A,cpu.E);
            break;

        case 0xA4:  // AND H
            std::cout << "AND H" << std::endl;
            and_(cpu.A,cpu.H);
            break;

        case 0xA5:  // AND L
            std::cout << "AND L" << std::endl;
            and_(cpu.A,cpu.L);
            break;

        case 0xA6:  // AND (HL)
            std::cout << "AND (HL)" << std::endl;
            and_(cpu.A,mmu.romData[cpu.HL]);
            break;

        case 0xA7:  // AND A
            std::cout << "AND A" << std::endl;
            and_(cpu.A,cpu.A);
            break;

        case 0xA8: // XOR B
            std::cout << "XOR B" << std::endl;
            xor_(cpu.A,cpu.B);
            break;

        case 0xA9:  // XOR C
            std::cout << "XOR C" << std::endl; 
            xor_(cpu.A,cpu.C);
            break;

        case 0xAA: // XOR D
            std::cout << "XOR D" << std::endl;
            xor_(cpu.A,cpu.D);
            break;

        case 0xAB: // XOR E
            std::cout << "XOR E" << std::endl; 
            xor_(cpu.A,cpu.E);
            break;

        case 0xAC: // XOR H
            std::cout << "XOR H" << std::endl;
            xor_(cpu.A,cpu.H);
            break;

        case 0xAD:  // XOR L
            std::cout << "XOR L" << std::endl;
            xor_(cpu.A,cpu.L);
            break;

        case 0xAE: // XOR (HL)
            std::cout << "XOR (HL)" << std::endl;
            xor_(cpu.A,mmu.romData[cpu.HL]);
            break;

        case 0xAF: // XOR A
            std::cout << "XOR A" << std::endl;
            xor_(cpu.A,cpu.A);
            break;

        case 0xB0: // OR B
            std::cout << "OR B" << std::endl;
            or_(cpu.A,cpu.B);
            break;

        case 0xB1:  // OR C
            std::cout << "OR C" << std::endl;
            or_(cpu.A,cpu.C);
            break;

        case 0xB2:  // OR D
            std::cout << "OR D" << std::endl;
            or_(cpu.A,cpu.D);
            break;

        case 0xB3:  // OR E
            std::cout << "OR E" << std::endl;
            or_(cpu.A,cpu.E);
            break;

        case 0xB4:  // OR H
            std::cout << "OR H" << std::endl;
            or_(cpu.A,cpu.H);
            break;

        case 0xB5:  // OR L
            std::cout << "OR L" << std::endl;
            or_(cpu.A,cpu.L);
            break;

        case 0xB6:  // OR (HL)
            std::cout << "OR (HL)" << std::endl;
            or_(cpu.A,mmu.romData[cpu.HL]);
            break;

        case 0xB7:  // OR A
            std::cout << "OR A" << std::endl;
            or_(cpu.A,cpu.A);
            break;

        case 0xB8:  // CP B
            std::cout << "CP B" << std::endl;
            cp_(cpu.A, cpu.B);
            break;

        case 0xB9: // CP C
            std::cout << "CP C" << std::endl;
            cp_(cpu.A, cpu.C);
            break;

        case 0xBA: // CP D
            std::cout << "CP D" << std::endl;
            cp_(cpu.A, cpu.D);
            break;

        case 0xBB:  // CP E
            std::cout << "CP D" << std::endl;
            cp_(cpu.A, cpu.E);
            break;

        case 0xBC:  // CP H
            std::cout << "CP H" << std::endl;
            cp_(cpu.A, cpu.H);
            break;

        case 0xBD:  // CP L
            std::cout << "CP L" << std::endl;
            cp_(cpu.A, cpu.L);
            break;

        case 0xBE:  // CP (HL)
            std::cout << "CP (HL)" << std::endl;
            cp_(cpu.A, mmu.romData[cpu.HL]);
            break;

        case 0xBF:  // CP A
            std::cout << "CP A" << std::endl;
            cp_(cpu.A, cpu.A);
            break;

        case 0xC0:  // RET NZ
            std::cout << "RET NZ" << std::endl;
            if (!(cpu.F & FLAG_ZERO)) {
                ret(true);
            }
            break;

        case 0xC1:  // POP BC
            std::cout << "POP BC" << std::endl;
            cpu.C = mmu.read8(cpu.SP);
            cpu.B = mmu.read8(cpu.SP + 1);
            cpu.SP += 2;
            break;

        case 0xC2:  // JP NZ, nn
            std::cout << "JP NZ, nn" << std::endl;
            if (!(cpu.F & FLAG_ZERO)) {
                cpu.PC = mmu.read8(cpu.PC) | mmu.read8(cpu.PC + 1) << 8;
            } else {
                cpu.PC += 2;
            }
            break;

        case 0xC3:  // JP nn
            std::cout << "JP nn" << std::endl;
            cpu.PC = mmu.read8(cpu.PC) | (mmu.read8(cpu.PC + 1) << 8);
            break;

        case 0xC4:  // CALL NZ, nn
            std::cout << "CALL nn" << std::endl;
            if (!(cpu.F & FLAG_ZERO)) {
                call(true);
            }
            break;
        case 0xC5:  // PUSH BC
            std::cout << "PUSH BC" << std::endl;
            cpu.SP -= 2;
            mmu.write8(cpu.SP + 1, cpu.B); 
            mmu.write8(cpu.SP, cpu.C);
            break;

        case 0xC6:  // ADD A, d8
            std::cout << "ADD A, d8" << std::endl;
            add8(cpu.A, mmu.read8(cpu.PC + 1));
            break;

        case 0xC7:  // RST 00h
            std::cout << "RST 00h" << std::endl;
            cpu.SP -= 2;  
            mmu.write8(cpu.SP, (cpu.PC >> 8) & 0xFF);
            mmu.write8(cpu.SP + 1, cpu.PC & 0xFF);
            break;

        case 0xC8:  // RET Z
            std::cout << "RET Z" << std::endl;
            if (cpu.F & FLAG_ZERO) {
                ret(true); 
            }
            break;

        case 0xC9:  // RET
            std::cout << "RET" << std::endl;
            ret(true);
            break;

        case 0xCA:  // JP Z, nn
            std::cout << "JP Z, nn" << std::endl;
            break;

        case 0xCB:  // PREFIX CB
            std::cout << "PREFIX CB" << std::endl;
            break;

        case 0xCC:  // CALL Z, nn
            std::cout << "PREFIX CB" << std::endl;
            break;

        case 0xCD:  // CALL nn
            std::cout << "CALL nn" << std::endl;
            break;
    }
}

void InstructionSet::ret(bool condition) {
    if (condition) {
        cpu.SP += 2;
        uint16_t address = mmu.read16(cpu.SP);
        cpu.PC = address;
    }
}

void InstructionSet::or_(uint8_t reg_1, uint8_t reg_2) {
    uint8_t tmp = reg_1 | reg_2;
    cpu.set_flag(FLAG_ZERO, (tmp == 0));
    cpu.clear_flag(FLAG_SUBTRACT);
    cpu.clear_flag(FLAG_HALF_CARRY);
    cpu.clear_flag(FLAG_CARRY);
    *reg_1 = tmp;
}

void inc_mem(uint8_t *value) {
    return;
}
void InstructionSet::cpl(uint8_t *reg) {
    reg = ~reg;

    cpu.set_flag(FLAG_SUBTRACT, true);
    cpu.set_flag(FLAG_HALF_CARRY, true);
}

void InstructionSet::inc_mem(uint16_t *reg) {
    uint_8_t tmp = mmu.read(*reg); 
    uint8_t nibble_carry = *tmp & 0x0F;
    tmp++;
    mmu.write8(*reg,tmp);

    set_flag(FLAG_HALF_CARRY,(nibble_carry == 0x0F));
    cpu.set_flag(FLAG_ZERO, (*_register == 0));
    cpu.clear_flag(FLAG_SUBTRACT);
}

void InstructionSet::inc(uint8_t *reg) {
    uint8_t nibble_carry = *reg & 0x0F;

    (*reg)++;
    set_flag(FLAG_HALF_CARRY,(nibble_carry == 0x0F));

    cpu.set_flag(FLAG_ZERO, (*reg == 0));
    cpu.clear_flag(FLAG_SUBTRACT);
}

void InstructionSet::inc(uint16_t *reg) {
    uint8_t nibble_carry = *reg & 0x0F;

    (*reg)++;
    set_flag(FLAG_HALF_CARRY,(nibble_carry == 0x0F));

    cpu.set_flag(FLAG_ZERO, (*reg == 0));
    cpu.clear_flag(FLAG_SUBTRACT);
}

void InstructionSet::dec(uint8_t *reg) {
    cpu.set_flag(FLAG_HALF_CARRY,(*reg & 0x0F) == 0);
    (*reg)--;

    cpu.set_flag(FLAG_ZERO, (*reg == 0));
    cpu.set_flag(FLAG_SUBTRACT, true);
}

void InstructionSet::add8(uint8_t *reg_1, uint8_t *reg_2) {
    uint8_t tmp = *reg_1 + *reg_2;

    cpu.set_flag(FLAG_CARRY, tmp > 0xFF);
    cpu.set_flag(FLAG_ZERO, (tmp == 0));
    cpu.set_flag(FLAG_HALF_CARRY, ((*reg_1 & 0x0F) + (*reg_2 & 0x0F)) > 0x0F);
    cpu.clear_flag(FLAG_SUBTRACT);
    *reg_1 = tmp & 0xFF;
}

void InstructionSet::add16(uint16_t *destination, uint16_t *value) {
    uint32_t result = *destination + *value;
    cpu.F &= ~FLAG_SUBTRACT;
    cpu.set_flag(FLAG_CARRY, result > 0xFFFF);

    cpu.set_flag(FLAG_HALF_CARRY, ((*destination & 0x0FFF) + (*value & 0x0FFF)) > 0x0FFF);
    *destination = result & 0xFFFF;
}

void ldhl(int8_t value) {
}

void InstructionSet::adc(uint8_t& reg_1, uint8_t reg_2) {
    uint8_t carry = cpu.get_flag(FLAG_CARRY) ? 1 : 0;

    uint16_t result = reg_1 + reg_2 + carry;

    reg_1 = result & 0xFF;

    cpu.set_flag(FLAG_ZERO, (reg_1 == 0));
    cpu.set_flag(FLAG_HALF_CARRY, ((reg_1 & 0x0F) + (reg_2 & 0x0F) + carry) > 0x0F);
    cpu.set_flag(FLAG_CARRY, (result > 0xFF));
    cpu.clear_flag(FLAG_SUBTRACT);
}
void sbc(uint8_t reg_1, uint8_t reg_2) {
    uint8_t carry = cpu.get_flag(FLAG_CARRY) ? 1 : 0;

    uint16_t result = reg_1 - reg_2 - carry;
    reg_1 = result & 0xFF;

    cpu.set_flag(FLAG_ZERO, (reg_1 == 0));
    cpu.set_flag(FLAG_SUBTRACT, 1);
    cpu.set_flag(FLAG_HALF_CARRY, ((reg_1 & 0x0F) < (reg_2 & 0x0F) + carry));  
    cpu.set_flag(FLAG_CARRY, (result > 0xFF));
}

void InstructionSet::sub(uint8_t reg_1, uint8_t reg_2) {
    uint16_t result = reg_1 - reg_2;

    reg_1 = result & 0xFF;

    cpu.set_flag(FLAG_ZERO, (reg_1 == 0));
    cpu.set_flag(FLAG_SUBTRACT, 1);
    cpu.set_flag(FLAG_HALF_CARRY, ((reg_1 & 0x0F) < (value & 0x0F)));
    cpu.set_flag(FLAG_CARRY, (result > 0xFF));
}

void InstructionSet::rlc(uint8_t *reg) {
    bool msb = *reg & 0x80;
    *reg = (*reg << 1) | (msb >> 7);

    cpu.set_flag(FLAG_CARRY, msb);

    cpu.set_flag(FLAG_ZERO, *reg == 0);

    cpu.clear_flag(FLAG_SUBTRACT);
    cpu.clear_flag(FLAG_HALF_CARRY);
}

void InstructionSet::rrca(uint8_t *reg){
    bool least_sig_bit = *reg & 1;
    cpu.A = *reg >> 1;
    //  cpu.set_flag(FLAG_CARRY, least_sig_bit);

    cpu.clear_flag(FLAG_ZERO);
    cpu.clear_flag(FLAG_SUBTRACT);
    cpu.clear_flag(FLAG_HALF_CARRY);
}
void and_(uint8_t &reg_1, uint8_t reg_2) {
    reg_1 = reg_1 & reg_2;

    cpu.set_flag(FLAG_ZERO, reg_1 == 0);
    cpu.clear_flag(FLAG_SUBTRACT);
    cpu.set_flag(FLAG_HALF_CARRY, 1);
    cpu.clear_flag(FLAG_CARRY);
}

void InstructionSet::dec_mem(uint16_t *reg) {
    uint8_t  tmp = mmu.read(*reg); 
    uint8_t nibble_carry = tmp & 0x0F;
    tmp--;
    mmu.write8(*reg,tmp);

    cpu.set_flag(FLAG_HALF_CARRY, nibble_carry == 0);
    cpu.set_flag(FLAG_ZERO, nibble_carry == 0);
    cpu.set_flag(FLAG_SUBTRACT, true);
}

void InstructionSet::rla() {
    std::cout << "RLA" << std::endl;
    bool carry = cpu.F & FLAG_CARRY; 
    cpu.set_flag(FLAG_CARRY, cpu.A & 0x80);
    cpu.A = cpu.A << 1;
    if (carry) {
        cpu.A |= 0x01; 
    }
}

void InstructionSet::add8_mem(uint8_t *destination, uint8_t value) {
    mmu.write8(*destination, *destination + value);

    cpu.set_flag(FLAG_ZERO, (*destination + value) == 0);
    cpu.clear_flag(FLAG_SUBTRACT);
    cpu.set_flag(FLAG_HALF_CARRY, ((*destination & 0x0F) + (value & 0x0F)) > 0x0F);
    cpu.set_flag(FLAG_CARRY, (*destination + value) > 0xFF);
}

void InstructionSet::or_(uint8_t &reg_1, uint8_t &reg_2) {
    uint8_t  tmp = reg_1 | reg_2;

    cpu.set_flag(FLAG_ZERO, tmp == 0);
    cpu.clear_flag(FLAG_CARRY);
    cpu.clear_flag(FLAG_SUBTRACT);
    cpu.clear_flag(FLAG_HALF_CARRY);
}

void InstructionSet::rra() {
    bool msb = cpu.A & 0x01;
    cpu.A = cpu.A >> 1;
    if (msb) {
        cpu.A |= 0x80;
    }

    cpu.clear_flag(FLAG_ZERO);
    cpu.clear_flag(FLAG_SUBTRACT);
    cpu.clear_flag(FLAG_HALF_CARRY);
    cpu.set_flag(FLAG_CARRY, msb);
}
void dec_mem(uint8_t *value) {
    return;
}

void InstructionSet::cp(uint8_t reg_1, uint8_t reg_2) {
    uint16_t tmp = reg_1 - reg_2;

    cpu.set_flag(FLAG_ZERO, (reg_1 == reg_2));
    cpu.set_flag(FLAG_SUBTRACT, 1);
    cpu.set_flag(FLAG_HALF_CARRY, ((reg_1 & 0x0F) < (reg_2 & 0x0F)));
    cpu.set_flag(FLAG_CARRY, (tmp > 0xFF));
}

void InstructionSet::call(bool condition) {
    if (condition) {
        uint16_t address = mmu.read(cpu.PC + 1);
        cpu.PC += 3;
        cpu.SP -= 2;
        mmu.write16(cpu.SP, cpu.PC);
        cpu.PC = address;
    } else {
        cpu.PC += 3;
    }
}


