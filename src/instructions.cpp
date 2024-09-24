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
            add(&cpu.HL, &cpu.BC);
            break;

        case 0x0A:  // LD A, (BC)
            std::cout << "LD A, (BC)" << std::endl;
            cpu.A = mmu.read(cpu.BC);
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
                    // TODO
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
            bool carry = cpu.F & FLAG_CARRY; 
            cpu.set_flag(FLAG_CARRY, cpu.A & 0x80);
            cpu.A = cpu.A << 1;
            if (carry) {
                cpu.A |= 0x01; 
            }
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
            add(&cpu.HL, &cpu.DE);
            break;

        case 0x1A:  // LD A, (DE)
            std::cout << "LD A, (DE)" << std::endl;
            cpu.A = mmu.read(cpu.DE);
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
                   // TODO
            std::cout << "RRA" << std::endl;
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
                    // TODO
            std::cout << "DAA" << std::endl;
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
            add(&cpu.HL, &cpu.HL);
            break;

        case 0x2A: // LD A, (HL+)
            std::cout << "LD A, (HL+)" << std::endl;
            cpu.A = mmu.read(cpu.HL);
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
                   // TODO
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

        case 0x36:  // LD (HL), d8
            break;

        case 0x37: 
            break;

        case 0x38: 
            break;

        case 0x39: 
            break;

        case 0x3A: 
            break;

        case 0x3B: 
            break;

        case 0x3C: 
            break;

        case 0x3D: 
            break;

        case 0x3E: 
            break;

        case 0x3F: 
            break;

        case 0x40: 
            break;

        case 0x41: 
            break;

        case 0x42: 
            break;

        case 0x43: 
            break;

        case 0x44: 
            break;

        case 0x45: 
            break;

        case 0x46: 
            break;

        case 0x47: 
            break;

        case 0x48: 
            break;

        case 0x49: 
            break;

        case 0x4A: 
            break;

        case 0x4B: 
            break;

        case 0x4C: 
            break;

        case 0x4D: 
            break;

        case 0x4E: 
            break;

        case 0x4F: 
            break;

        case 0x50: 
            break;

        case 0x51: 
            break;

        case 0x52: 
            break;

        case 0x53: 
            break;

        case 0x54: 
            break;

        case 0x55: 
            break;

        case 0x56: 
            break;

        case 0x57: 
            break;

        case 0x58: 
            break;

        case 0x59: 
            break;

        case 0x5A: 
            break;

        case 0x5B: 
            break;

        case 0x5C: 
            break;

        case 0x5D: 
            break;

        case 0x5E: 
            break;

        case 0x5F: 
            break;

        case 0x60: 
            break;

        case 0x61: 
            break;

        case 0x62: 
            break;

        case 0x63: 
            break;

        case 0x64: 
            break;

        case 0x65: 
            break;

        case 0x66: 
            break;

        case 0x67: 
            break;

        case 0x68: 
            break;

        case 0x69: 
            break;

        case 0x6A: 
            break;

        case 0x6B: 
            break;

        case 0x6C: 
            break;

        case 0x6D: 
            break;

        case 0x6E: 
            break;

        case 0x6F: 
            break;

        case 0x70: 
            break;

        case 0x71: 
            break;

        case 0x72: 
            break;

        case 0x73: 
            break;

        case 0x74: 
            break;

        case 0x75: 
            break;

        case 0x76: 
            break;

        case 0x77: 
            break;

        case 0x78: 
            break;

        case 0x79: 
            break;

        case 0x7A: 
            break;

        case 0x7B: 
            break;

        case 0x7C: 
            break;

        case 0x7D: 
            break;

        case 0x7E: 
            break;

        case 0x7F: 
            break;

        case 0x80: 
            break;
        case 0x81: 
            break;

        case 0x82: 
            break;

        case 0x83: 
            break;

        case 0x84: 
            break;

        case 0x85: 
            break;

        case 0x86: 
            break;

        case 0x87: 
            break;

        case 0x88: 
            break;

        case 0x89: 
            break;

        case 0x8A: 
            break;

        case 0x8B: 
            break;

        case 0x8C: 
            break;

        case 0x8D: 
            break;

        case 0x8E: 
            break;

        case 0x8F: 
            break;

        case 0x90: 
            break;

        case 0x91: 
            break;

        case 0x92: 
            break;

        case 0x93: 
            break;

        case 0x94: 
            break;

        case 0x95: 
            break;

        case 0x96: 
            break;

        case 0x97: 
            break;

        case 0x98: 
            break;

        case 0x99: 
            break;

        case 0x9A: 
            break;

        case 0x9B: 
            break;

        case 0x9C: 
            break;

        case 0x9D: 
            break;

        case 0x9E: 
            break;

        case 0x9F: 
            break;

        case 0xA0: 
            break;

        case 0xA1: 
            break;

        case 0xA2: 
            break;

        case 0xA3: 
            break;

        case 0xA4: 
            break;

        case 0xA5: 
            break;

        case 0xA6: 
            break;

        case 0xA7: 
            break;

        case 0xA8: 
            break;

        case 0xA9: 
            break;

        case 0xAA: 
            break;

        case 0xAB: 
            break;

        case 0xAC: 
            break;

        case 0xAD: 
            break;

        case 0xAE: 
            break;

        case 0xAF: 
            break;

        case 0xB0: 
            break;

        case 0xB1: 
            break;

        case 0xB2: 
            break;

        case 0xB3: 
            break;

        case 0xB4: 
            break;

        case 0xB5: 
            break;

        case 0xB6: 
            break;

        case 0xB7: 
            break;

        case 0xB8: 
            break;

        case 0xB9: 
            break;

        case 0xBA: 
            break;

        case 0xBB: 
            break;

        case 0xBC: 
            break;

        case 0xBD: 
            break;

        case 0xBE: 
            break;

        case 0xBF: 
            break;

        case 0xC0: 
            break;

        case 0xC1: 
            break;

        case 0xC2: 
            break;

        case 0xC3: 
            break;

        case 0xC4: 
            break;

        case 0xC5: 
            break;

        case 0xC6: 
            break;

        case 0xC7: 
            break;

        case 0xC8: 
            break;

        case 0xC9: 
            break;

        case 0xCA: 
            break;

        case 0xCB: 
            break;

        case 0xCC: 
            break;

        case 0xCD: 
            break;

        case 0xCE: 
            break;

        case 0xCF: 
            break;

        case 0xD0: 
            break;

        case 0xD1: 
            break;

        case 0xD2: 
            break;

        case 0xD3: 
            break;

        case 0xD4: 
            break;

        case 0xD5: 
            break;

        case 0xD6: 
            break;

        case 0xD7: 
            break;

        case 0xD8: 
            break;

        case 0xD9: 
            break;

        case 0xDA: 
            break;

        case 0xDB: 
            break;

        case 0xDC: 
            break;

        case 0xDD: 
            break;

        case 0xDE: 
            break;

        case 0xDF: 
            break;

        case 0xE0: 
            break;

        case 0xE1: 
            break;

        case 0xE2: 
            break;

        case 0xE3: 
            break;

        case 0xE4: 
            break;

        case 0xE5: 
            break;

        case 0xE6: 
            break;

        case 0xE7: 
            break;

        case 0xE8: 
            break;

        case 0xE9: 
            break;

        case 0xEA: 
            break;

        case 0xEB: 
            break;

        case 0xEC: 
            break;

        case 0xED: 
            break;

        case 0xEE: 
            break;

        case 0xEF: 
            break;

        case 0xF0: 
            break;

        case 0xF1: 
            break;

        case 0xF2: 
            break;

        case 0xF3: 
            break;

        case 0xF4: 
            break;

        case 0xF5: 
            break;

        case 0xF6: 
            break;

        case 0xF7: 
            break;

        case 0xF8: 
            break;

        case 0xF9: 
            break;

        case 0xFA: 
            break;

        case 0xFB: 
            break;

        case 0xFC: 
            break;

        case 0xFD: 
            break;

        case 0xFE: 
            break;

        case 0xFF: 
            break;

        default:
            std::cout << "Invalid Instruction: " << opcode << std::endl;
            break;
    }
}

void ret(bool condition) {
}
void xor_(uint8_t value) {
}
void inc_mem(uint8_t *value) {
    return  
}

void inc_mem(uint16_t *reg) {
    uint_8_t tmp = mmu.read(*reg); 
    uint8_t nibble_carry = *tmp & 0x0F;
    tmp++;
    mmu.write8(*reg,tmp);

    set_flag(FLAG_HALF_CARRY,(nibble_carry == 0x0F));
    cpu.set_flag(FLAG_ZERO, (*_register == 0));
    cpu.clear_flag(FLAG_SUBTRACT);
}

void inc(uint8_t *reg) {
    uint8_t nibble_carry = *reg & 0x0F;

    (*reg)++;
    set_flag(FLAG_HALF_CARRY,(nibble_carry == 0x0F));

    cpu.set_flag(FLAG_ZERO, (*reg == 0));
    cpu.clear_flag(FLAG_SUBTRACT);
}

void inc(uint16_t *reg) {
    uint8_t nibble_carry = *reg & 0x0F;

    (*reg)++;
    set_flag(FLAG_HALF_CARRY,(nibble_carry == 0x0F));

    cpu.set_flag(FLAG_ZERO, (*reg == 0));
    cpu.clear_flag(FLAG_SUBTRACT);
}

void dec(uint8_t *reg) {
    cpu.set_flag(FLAG_HALF_CARRY,(*reg & 0x0F) == 0);
    (*reg)--;

    cpu.set_flag(FLAG_ZERO, (*reg == 0));
    cpu.set_flag(FLAG_SUBTRACT, true);
}

void add(uint8_t *destination, uint8_t value) {
}

void add(uint16_t *destination, uint16_t *value) {
    uint32_t result = *destination + *value;
    cpu.F &= ~FLAG_SUBTRACT;
    cpu.set_flag(FLAG_CARRY, result > 0xFFFF);

    cpu.set_flag(FLAG_HALF_CARRY, ((*destination & 0x0FFF) + (*value & 0x0FFF)) > 0x0FFF);
    *destination = result & 0xFFFF;
}

void ldhl(int8_t value) {
}
void adc(uint8_t value) {
}
void sbc(uint8_t value) {
}
void sub(uint8_t value) {
}
void and_(uint8_t value) {
}
void or_(uint8_t value) {
}
void cp(uint8_t value) {
}
void call(bool condition) {
}
void jump(bool condition) {
}
void jump_add(bool condition) {
}
void cp_n(uint8_t value) {
}
void rlc(uint8_t *reg) {
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

void dec_mem(uint16_t *reg) {

    uint8_t  tmp = mmu.read(*reg); 
    uint8_t nibble_carry = tmp & 0x0F;
    tmp--;
    mmu.write8(*reg,tmp);

    cpu.set_flag(FLAG_HALF_CARRY, nibble_carry == 0);
    cpu.set_flag(FLAG_ZERO, nibble_carry == 0);
    cpu.set_flag(FLAG_SUBTRACT, true);
}

void dec_mem(uint8_t *value) {
    return;
}
void extended_execute(uint8_t opcode) {
}
void bit(uint8_t bit, uint8_t value) {
}
void res(uint8_t bit, uint8_t *rgst) {
}
void set(uint8_t bit, uint8_t *rgst) {
}
void rl(uint8_t *value) {
}
void rlc(uint8_t *value) {
}
void rr(uint8_t *value) {
}
void rrc(uint8_t *value) {
}
void rra() {
}
void rla() {
}
void rlca() {
}
void sla(uint8_t *value) {
}
void sra(uint8_t *value) {
}
void srl(uint8_t *value) {
}
void swap(uint8_t *value) {
