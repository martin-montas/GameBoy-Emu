// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#include "instructions.hpp"
// #include "MBC.hpp"
// #include "MBC0.hpp"
// #include "cpu.hpp"

#include <bit>
#include <cstdint>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

#include "cpu.hpp"
#include "bus.hpp"

void InstructionSet::pre_boot_state() {
    _cpu->PC = 0x0000;
    _cpu->SP = 0xFFFE;

    _cpu->A = 0x01;
    _cpu->F = 0xB0;
    _cpu->B = 0x00;
    _cpu->C = 0x13;
    _cpu->D = 0x00;
    _cpu->E = 0xD8;
    _cpu->H = 0x01;
    _cpu->L = 0x4D;

    _cpu->_ime = 0;

    _bus->write8(0xFF01, 0x00); // SB (Serial Link Data Buffer)
    _bus->write8(0xFF02, 0x7E); // SC (Serial Link Control - Unused bits read as 1)

    // 2. Timers & System Interrupts
    _bus->write8(0xFF04, 0x00); // DIV (Divider Register)
    _bus->write8(0xFF05, 0x00); // TIMA
    _bus->write8(0xFF06, 0x00); // TMA
    _bus->write8(0xFF07, 0x00); // TAC

    // 3. Audio Registers (APU)
    _bus->write8(0xFF10, 0x00);
    _bus->write8(0xFF11, 0x00);
    _bus->write8(0xFF12, 0x00);
    _bus->write8(0xFF14, 0x00);
    _bus->write8(0xFF16, 0x00);
    _bus->write8(0xFF17, 0x00);
    _bus->write8(0xFF19, 0x00);
    _bus->write8(0xFF1A, 0x00);
    _bus->write8(0xFF1B, 0x00);
    _bus->write8(0xFF1C, 0x00);
    _bus->write8(0xFF1E, 0x00);
    _bus->write8(0xFF20, 0x00);
    _bus->write8(0xFF21, 0x00);
    _bus->write8(0xFF22, 0x00);
    _bus->write8(0xFF23, 0x00); // Fixed duplicate typo here
    _bus->write8(0xFF24, 0x00);
    _bus->write8(0xFF25, 0x00);
    _bus->write8(0xFF26, 0x00);

    // 4. Graphics Registers (PPU) - CRITICAL ADDITIONS
    _bus->write8(0xFF40, 0x91); // LCDC
    _bus->write8(0xFF41, 0x85); // STAT
    _bus->write8(0xFF42, 0x00); // SCY
    _bus->write8(0xFF43, 0x00); // SCX
    _bus->write8(0xFF44, 0x00); // LY
    _bus->write8(0xFF45, 0x00); // LYC
    _bus->write8(0xFF47, 0xFC); // BGP
    _bus->write8(0xFF48, 0xFF); // OBP0
    _bus->write8(0xFF49, 0xFF); // OBP1
    _bus->write8(0xFF4A, 0x00); // WY
    _bus->write8(0xFF4B, 0x00); // WX

    // 5. Unmap Boot ROM
    // _bus->write8(0xFF50, 0x01); // Unmaps Boot ROM, enabling Cartridge mapping

    // 6. Interrupt Enable
    _bus->write8(0xFF0F, 0xE1);
    _bus->write8(0xFFFF, 0x00);
}
void InstructionSet::post_boot_state() {
    // =========================
    // CPU state after boot ROM
    // =========================
    _cpu->PC = 0x0100;
    _cpu->SP = 0xFFFE;

    _cpu->A = 0x01;
    _cpu->F = 0xB0;
    _cpu->B = 0x00;
    _cpu->C = 0x13;
    _cpu->D = 0x00;
    _cpu->E = 0xD8;
    _cpu->H = 0x01;
    _cpu->L = 0x4D;

    _cpu->_ime = 0;

    _bus->write8(0xFF0F, 0x00); // IF
    _bus->write8(0xFFFF, 0x00); // IE

    _bus->write8(0xFF04, 0x00); // DIV
    _bus->write8(0xFF05, 0x00); // TIMA
    _bus->write8(0xFF06, 0x00); // TMA
    _bus->write8(0xFF07, 0x00); // TAC

    _bus->write8(0xFF01, 0x00);
    _bus->write8(0xFF02, 0x7E);

    _bus->write8(0xFF10, 0x80);
    _bus->write8(0xFF11, 0xBF);
    _bus->write8(0xFF12, 0xF3);
    _bus->write8(0xFF14, 0xBF);
    _bus->write8(0xFF16, 0x3F);
    _bus->write8(0xFF17, 0x00);
    _bus->write8(0xFF19, 0xBF);
    _bus->write8(0xFF1A, 0x7F);
    _bus->write8(0xFF1B, 0xFF);
    _bus->write8(0xFF1C, 0x9F);
    _bus->write8(0xFF1E, 0xBF);
    _bus->write8(0xFF20, 0xFF);
    _bus->write8(0xFF21, 0x00);
    _bus->write8(0xFF22, 0x00);
    _bus->write8(0xFF23, 0xBF);
    _bus->write8(0xFF24, 0x77);
    _bus->write8(0xFF25, 0xF3);
    _bus->write8(0xFF26, 0xF1); // NR52

    _bus->write8(0xFF40, 0x91); // LCDC
    _bus->write8(0xFF41, 0x85); // STAT
    _bus->write8(0xFF42, 0x00); // SCY
    _bus->write8(0xFF43, 0x00); // SCX
    _bus->write8(0xFF44, 0x00); // LY
    _bus->write8(0xFF45, 0x00); // LYC

    _bus->write8(0xFF47, 0xFC); // BGP
    _bus->write8(0xFF48, 0xFF); // OBP0
    _bus->write8(0xFF49, 0xFF); // OBP1

    _bus->write8(0xFF4A, 0x00); // WY
    _bus->write8(0xFF4B, 0x00); // WX

    // =========================
    // PPU internal state
    // =========================
    // ppu.mode        = 2; // OAM scan
    // ppu.dots        = 0;
    // ppu.window_line = 0;

    // // optional but useful for determinism
    // ppu.line_ticks = 0;
}

void InstructionSet::ldr(uint16_t& reg) {
    // DONE
    uint8_t  tmp_1 = this->_bus->read8(_cpu->PC + 1);
    uint8_t  tmp_2 = this->_bus->read8(_cpu->PC + 2);
    uint16_t tmp   = (tmp_2 << 8) | tmp_1;
    reg            = tmp;
}

void InstructionSet::ldr(uint8_t& reg) {
    uint8_t tmp = this->_bus->read8(this->_cpu->PC + 1);
    reg         = tmp;
}

// DONE:
void InstructionSet::ld_mem(uint8_t& reg, uint16_t addr) {
    _bus->write8(addr, reg);
}

void InstructionSet::jump(bool condition, int8_t offset) {
    // DONE:
    if (condition) {
        _cpu->PC += offset;
    }
}
void InstructionSet::inc(uint16_t& reg) {
    uint16_t old = reg;

    reg = old + 1;

    _cpu->clear_flag(FLAG_SUBTRACT);
    _cpu->set_flag(FLAG_ZERO, reg == 0);
    _cpu->set_flag(FLAG_HALF_CARRY, ((old & 0x0FFF) + 1) > 0x0FFF);
}
void InstructionSet::ld(uint16_t& reg, uint16_t addr) {
    reg = addr;
}
void InstructionSet::ld(uint8_t& reg, uint8_t addr) {
    reg = addr;
}

void InstructionSet::execute(uint8_t opcode) {
    // printf("PC:%04X HL:%04X ", _cpu->PC, _cpu->HL);
    switch (opcode) {
    case 0x00: {
        _cpu->PC += 1;
        // printf("NOP  -- %X --\n", _cpu->PC);
        break;
    }
    case 0x01: {
        uint8_t  l   = _bus->read8(_cpu->PC + 1);
        uint8_t  h   = _bus->read8(_cpu->PC + 2);
        uint16_t val = (h << 8) | l;
        // printf("LD BC, d16 0x01 -- %X --\n", val);
        _cpu->BC = val;
        _cpu->PC = _cpu->PC + 3;
        break;
    }
    case 0x02: {
        _bus->write8(_cpu->BC, _cpu->A);
        // printf("LD (BC), A 0x02 -- %X --\n", _cpu->BC);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x03: {
        // printf("INC BC 0x03 -- before %X --\n", _cpu->BC);
        _cpu->BC = _cpu->BC + 1;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x04: {
        // printf("INC B 0x04 -- before %X --\n", _cpu->B);
        inc(_cpu->B);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x05: {
        // printf("DEC B 0x05 -- before %X --\n", _cpu->B);
        dec(_cpu->B);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x06: {
        // printf("LD B, d8 0x06 --- before %X --\n", _cpu->B);
        _cpu->B  = _bus->read8(_cpu->PC + 1);
        _cpu->PC = _cpu->PC + 2;
        break;
    }
    case 0x07: {

        bool carry = (_cpu->A & 0x80);

        _cpu->A = static_cast<uint8_t>((_cpu->A << 1) | carry);

        _cpu->set_flag(FLAG_ZERO, false);
        _cpu->set_flag(FLAG_SUBTRACT, false);
        _cpu->set_flag(FLAG_HALF_CARRY, false);
        _cpu->set_flag(FLAG_CARRY, carry);

        _cpu->PC += 1;
        break;
    }
    case 0x08: {
        uint8_t  l    = _bus->read8(_cpu->PC + 1);
        uint8_t  h    = _bus->read8(_cpu->PC + 2);
        uint16_t addr = (h << 8) | l;

        // printf("LD (a16), SP -- %04X --\n", addr);

        uint16_t sp = _cpu->SP;

        _bus->write8(addr, sp & 0xFF);
        _bus->write8(addr + 1, (sp >> 8) & 0xFF);

        _cpu->PC += 3;
        break;
    }
    case 0x09: {
        // printf("ADD HL, BC -- %X --\n", _cpu->HL);
        _cpu->set_flag(FLAG_CARRY, (static_cast<uint32_t>(_cpu->HL) + _cpu->BC) > 0xFFFF);
        _cpu->set_flag(FLAG_SUBTRACT, false);
        _cpu->set_flag(FLAG_HALF_CARRY, ((_cpu->HL & 0x0FFF) + (_cpu->BC & 0x0FFF)) > 0x0FFF);
        _cpu->HL = _cpu->HL + _cpu->BC;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x0A: {
        // printf("LD A, (BC)\n");
        _cpu->A  = _bus->read8(_cpu->BC);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x0B: {
        _cpu->BC = _cpu->BC - 1;
        // printf("DEC BC -- %X --\n", _cpu->BC);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x0C: {
        inc(_cpu->C);
        // printf("INC C -- %X --\n", _cpu->C);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x0D: {
        dec(_cpu->C);
        // printf("DEC C -- %X --\n", _cpu->C);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x0E: {
        _cpu->C = _bus->read8(_cpu->PC + 1);
        // printf("LD C, d8 -- %X --\n", _cpu->C);
        _cpu->PC = _cpu->PC + 2;
        break;
    }
    case 0x0F: {
        rrca(_cpu->A);
        // printf("RRCA --%X --", _cpu->A);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x10: {
        // this one should be checked out
        // printf("STOP 0x10 : shouldn't be IMPLEMENTED \n");
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x11: {
        ldr(_cpu->DE);
        // printf("LD (DE), d16 -- %X --\n", _cpu->DE);
        _cpu->PC = _cpu->PC + 3;
        break;
    }
    case 0x12: {
        // printf("LD (DE), A -- %X --\n", _cpu->DE);
        _bus->write8(_cpu->DE, _cpu->A);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x13: {
        _cpu->DE = _cpu->DE + 1;
        // printf("INC DE -- %X --\n", _cpu->DE);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x14: {
        inc(_cpu->D);
        // printf("INC D, -- %X --\n", _cpu->D);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x15: {
        dec(_cpu->D);
        // printf("DEC D -- %X --\n", _cpu->D);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x16: {
        _cpu->D = _bus->read8(_cpu->PC + 1);
        // printf("LD D, d8 -- %X --\n", _cpu->D);
        _cpu->PC = _cpu->PC + 2;
        break;
    }
    case 0x17: {
        // printf("RLA\n");
        bool b7 = (_cpu->A & 0x80);

        bool c = _cpu->F & FLAG_CARRY;

        _cpu->A = static_cast<uint8_t>((_cpu->A << 1) | c);

        _cpu->set_flag(FLAG_ZERO, false);
        _cpu->set_flag(FLAG_SUBTRACT, false);
        _cpu->set_flag(FLAG_HALF_CARRY, false);
        _cpu->set_flag(FLAG_CARRY, b7);

        _cpu->PC += 1;
        break;

        break;
    }
    case 0x18: {
        int8_t offset;
        offset = (int8_t)_bus->read8(_cpu->PC + 1);
        _cpu->PC += 2;
        _cpu->PC += offset;
        // printf("JR r8 -- %X --\n", offset);
        break;
    }
    case 0x19: {
        add16(_cpu->HL, _cpu->DE);
        _cpu->PC = _cpu->PC + 1;
        // printf("ADD HL, DE --%X --\n", _cpu->HL);
        break;
    }
    case 0x1A: {
        _cpu->A = _bus->read8(_cpu->DE);
        // printf("LD A, (DE) -- %X --\n", _cpu->A);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x1B: {
        _cpu->DE = _cpu->DE - 1;
        // printf("DEC DE -- %X --\n", _cpu->DE);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x1C: {
        inc(_cpu->E);
        // printf("INC E -- %X --\n", _cpu->E);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x1D: {
        dec(_cpu->E);
        // printf("DEC E -- %X --\n", _cpu->E);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x1E: {
        // printf("LD E, d8");
        _cpu->E  = _bus->read8(_cpu->PC + 1);
        _cpu->PC = _cpu->PC + 2;
        break;
    }
    case 0x1F: {
        // printf("RRA\n");
        rra();
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x20: {
        // printf("JR NZ, s8\n");
        int8_t offset = (int8_t)(_bus->read8(_cpu->PC + 1));
        if (!(_cpu->F & FLAG_ZERO))
            _cpu->PC += 2 + offset;
        else
            _cpu->PC += 2;
        break;
    }
    case 0x21: {
        uint8_t l = _bus->read8(_cpu->PC + 1);
        uint8_t h = _bus->read8(_cpu->PC + 2);

        uint16_t v = (h << 8) | l;
        ld(_cpu->HL, v);
        // printf("LD HL, d16 0x21 -- (HL == %X) --\n", _cpu->HL);
        _cpu->PC = _cpu->PC + 3;
        break;
    }
    case 0x22: {
        // printf("LD (HL+), A");
        _bus->write8(_cpu->HL, _cpu->A);
        _cpu->HL += 1;
        _cpu->PC += 1;
        break;
    }
    case 0x23: {
        // printf("INC HL 0x23\n");
        _cpu->HL = _cpu->HL + 1;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x24: {
        // printf("INC H\n");
        inc(_cpu->H);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x25: {
        // printf("DEC H");
        dec(_cpu->H);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x26: {
        // printf("LD H, d8");
        ld(_cpu->H, _bus->read8(_cpu->PC + 1));
        _cpu->PC = _cpu->PC + 2;
        break;
    }
    case 0x27: {
        // printf("DAA");
        uint8_t correction = 0;
        if (!(_cpu->F & FLAG_SUBTRACT)) {
            if ((_cpu->A & 0x0F) > 9 || _cpu->F & FLAG_HALF_CARRY) {
                correction |= 0x06;
            }
            if ((_cpu->A > 0x99) || _cpu->F & FLAG_CARRY) {
                correction |= 0x60;
                _cpu->set_flag(FLAG_CARRY, true);
            }
        } else {
            if (_cpu->F & FLAG_HALF_CARRY)
                correction |= 0x06;
            if (_cpu->F & FLAG_CARRY)
                correction |= 0x60;
        }
        if (_cpu->F & FLAG_SUBTRACT)
            _cpu->A -= correction;
        else
            _cpu->A += correction;
        _cpu->set_flag(FLAG_ZERO, _cpu->A == 0);
        _cpu->clear_flag(FLAG_HALF_CARRY);
        _cpu->PC += 1;
        break;
    }
    case 0x28: {
        // printf("JR Z, r8 0x28");
        int8_t offset = (int8_t)_bus->read8(_cpu->PC + 1);

        if (_cpu->F & FLAG_ZERO)
            _cpu->PC += 2 + offset;
        else
            _cpu->PC += 2;

        break;
    }
    case 0x29: {
        // printf("ADD HL, HL\n");
        add16(_cpu->HL, _cpu->HL);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x2A: {
        // printf("LD A, (HL+)\n");
        uint8_t n8 = _bus->read8(_cpu->HL);
        _cpu->A    = n8;
        _cpu->HL += 1;
        _cpu->PC += 1;
        break;
    }
    case 0x2B: {
        // printf("DEC HL\n");
        _cpu->HL -= 1;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x2C: {
        // printf("INC L\n");
        inc(_cpu->L);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x2D: {
        // printf("DEC L\n");
        dec(_cpu->L);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x2E: {
        // printf("LD L, d8\n");
        _cpu->L  = _bus->read8(_cpu->PC + 1);
        _cpu->PC = _cpu->PC + 2;
        break;
    }
    case 0x2F: {
        // printf("CPL\n");
        cpl(_cpu->A);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x30: {
        // printf("JR NC,r8 -- --\n");
        jump(!(_cpu->F & FLAG_CARRY), _bus->read8(_cpu->PC + 1));
        _cpu->PC += 2;
        break;
    }
    case 0x31: {
        uint16_t l = _bus->read8(_cpu->PC + 1);
        uint16_t h = _bus->read8(_cpu->PC + 2);
        _cpu->SP   = (h << 8) | l;

        _cpu->PC = _cpu->PC + 3;
        break;
    }
    case 0x32: {
        // printf("LD (HL-), A\n");
        _bus->write8(_cpu->HL, _cpu->A);
        _cpu->HL -= 1;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x33: {

        // printf("INC SP\n");
        _cpu->SP = _cpu->SP + 1;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x34: {

        // printf("INC (HL) 0x34\n");
        inc_mem(_cpu->HL);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x35: {

        dec_mem(_cpu->HL);
        // printf("DEC (HL) 0x35 0x%X \n", _bus->read8(_cpu->HL));
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x36: {

        // printf("LD (HL), d8\n");
        _bus->write8(_cpu->HL, _bus->read8(_cpu->PC + 1));
        _cpu->PC = _cpu->PC + 2;
        break;
    }
    case 0x37: {

        // printf("SCF\n");
        _cpu->set_flag(FLAG_CARRY, true);
        _cpu->clear_flag(FLAG_SUBTRACT);
        _cpu->clear_flag(FLAG_HALF_CARRY);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x38: {
        int8_t offset = static_cast<int8_t>(_bus->read8(_cpu->PC + 1));

        if (_cpu->F & FLAG_CARRY) {
            _cpu->PC += 2 + offset;
        } else {
            _cpu->PC += 2;
        }

        break;
    }
    case 0x39: {
        // printf("ADD HL, SP\n");
        add16(_cpu->HL, _cpu->SP);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x3A: {
        // printf("LD A, (HL-)\n");
        _cpu->A  = _bus->read8(_cpu->HL);
        _cpu->HL = _cpu->HL - 1;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x3B: {
        // printf("DEC SP\n");
        _cpu->SP = _cpu->SP - 1;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x3C: {
        // printf("INC A\n");
        inc(_cpu->A);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x3D: {
        dec(_cpu->A);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x3E: {

        _cpu->A = _bus->read8(_cpu->PC + 1);
        // printf("LD A, d8: 0x3E A: -- %X -- \n", _cpu->A);
        _cpu->PC = _cpu->PC + 2;
        break;
    }
    case 0x3F: {

        // printf("CCF\n");
        bool carry = _cpu->F & FLAG_CARRY;
        _cpu->set_flag(FLAG_CARRY, !carry);
        _cpu->set_flag(FLAG_SUBTRACT, false);
        _cpu->set_flag(FLAG_HALF_CARRY, false);
        _cpu->PC += 1;

        break;
    }
    case 0x40: {

        // printf("CCFLD B, B\n");
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x41: {
        // printf("LD B, C\n");
        _cpu->B  = _cpu->C;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x42: {
        // printf("LD B, D\n");
        _cpu->B  = _cpu->D;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x43: {
        // printf("LD B, E\n");
        _cpu->B  = _cpu->E;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x44: {
        // printf("LD B, H\n");
        _cpu->B  = _cpu->H;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x45: {
        // printf("LD B, L\n");
        _cpu->B  = _cpu->L;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x46: {
        // printf("LD B, (HL)");
        _cpu->B  = _bus->read16(_cpu->HL);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x47: {
        // printf("LD B, A\n");
        _cpu->B  = _cpu->A;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x48: {
        // printf("LD C, B\n");
        _cpu->C  = _cpu->B;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x49: {
        // printf("LD C, C\n");
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x4A: {
        // printf("LD C, D\n");
        _cpu->C  = _cpu->D;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x4B: {
        // printf("LD C, E\n");
        _cpu->C  = _cpu->E;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x4C: {
        // printf("LD C, H\n");
        _cpu->C  = _cpu->H;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x4D: {
        // printf("LD C, L\n");
        _cpu->C  = _cpu->L;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x4E: {
        // printf("LD C, (HL)\n");
        _cpu->C  = _bus->read8(_cpu->HL);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x4F: {
        // printf("LD C, A\n");
        _cpu->C  = _cpu->A;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x50: {
        // printf("LD D, B\n");
        _cpu->D  = _cpu->B;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x51: {
        // printf("LD D, C\n");
        _cpu->D  = _cpu->C;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x52: {
        // printf("LD C, A\n");
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x53: {
        // printf("LD D, E\n");
        _cpu->D  = _cpu->E;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x54: {
        // printf("LD D, H\n");
        _cpu->D  = _cpu->H;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x55: {
        // printf("LD D, L\n");
        _cpu->D  = _cpu->L;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x56: {
        // printf("LD D, (HL)\n");

        _cpu->D  = _bus->read8(_cpu->HL);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x57: {
        // printf("LD D, A\n");
        _cpu->D  = _cpu->A;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x58: {
        // printf("LD D, B\n");
        _cpu->E  = _cpu->B;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x59: {
        // printf("LD E, C\n");
        _cpu->E  = _cpu->C;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x5A: {
        // printf("LD E, D\n");
        _cpu->E  = _cpu->D;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x5B: {
        // printf("LD D, B\n");
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x5C: {
        // printf("LD E, H\n");
        _cpu->E  = _cpu->H;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x5D: {
        // printf("LD E, L\n");
        _cpu->E  = _cpu->L;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x5E: {
        // printf("LD E, L\n");
        _cpu->E  = _bus->read8(_cpu->HL);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x5F: {
        // printf("LD E, A\n");
        _cpu->E  = _cpu->A;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x60: {
        // printf("LD H, B\n");
        _cpu->H  = _cpu->B;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x61: {
        // printf("LD H, C\n");
        _cpu->H  = _cpu->C;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x62: {
        // printf("LD H, D\n");
        _cpu->H  = _cpu->D;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x63: {
        // printf("LD H, E\n");
        _cpu->H  = _cpu->E;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x64: {
        // printf("LD H, H\n");
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x65: {
        // printf("LD H, L\n");
        _cpu->H  = _cpu->L;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x66: {
        // printf("LD H, (HL)\n");
        _cpu->H  = _bus->read8(_cpu->HL);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x67: {
        // printf("LD H, A\n");
        _cpu->H  = _cpu->A;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x68: {
        // printf("LD L, B\n");
        _cpu->L  = _cpu->B;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x69: {
        // printf("LD L, C\n");
        _cpu->L  = _cpu->C;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x6A: {
        // printf("LD L, D\n");
        _cpu->L  = _cpu->D;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x6B: {
        // printf("LD L, E\n");
        _cpu->L  = _cpu->E;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x6C: {
        // printf("LD L, H\n");
        _cpu->L  = _cpu->H;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x6D: {
        // printf("LD L, L\n");
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x6E: {
        // printf("LD L, (LH)\n");
        _cpu->L  = _bus->read8(_cpu->HL);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x6F: {
        // printf("LD L, A\n");
        _cpu->L  = _cpu->A;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x70: {
        // printf("LD (HL), B\n");
        _bus->write8(_cpu->HL, _cpu->B);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x71: {
        // printf("LD (HL), C\n");
        _bus->write8(_cpu->HL, _cpu->C);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x72: {
        // printf("LD (HL), D\n");
        _bus->write8(_cpu->HL, _cpu->D);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x73: {
        // printf("LD (HL), E\n");
        _bus->write8(_cpu->HL, _cpu->E);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x74: {
        // printf("LD (HL), H\n");
        _bus->write8(_cpu->HL, _cpu->H);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x75: {
        // printf("LD (HL), L\n");
        _bus->write8(_cpu->HL, _cpu->L);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x76: {
        _cpu->halted = true;
        _cpu->PC     = _cpu->PC + 1;
        break;
    }
    case 0x77: {
        // printf("LD (HL), A\n");
        _bus->write8(_cpu->HL, _cpu->A);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x78: {
        // printf("LD A, B\n");
        _cpu->A  = _cpu->B;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x79: {
        // printf("LD A, C\n");
        _cpu->A  = _cpu->C;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x7A: {
        // printf("LD A, D\n");
        _cpu->A  = _cpu->D;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x7B: {
        // printf("LD A, E\n");
        _cpu->A  = _cpu->E;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x7C: {
        // printf("LD A, H\n");
        _cpu->A  = _cpu->H;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x7D: {
        // printf("LD A, L\n");
        _cpu->A  = _cpu->L;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x7E: {
        // printf("LD A, (HL)\n");
        _cpu->A  = _bus->read8(_cpu->HL);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x7F: {
        // printf("LD A, A\n");
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x80: {
        // printf("ADD A, B\n");
        add8(_cpu->A, _cpu->B);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x81: {
        // printf("ADD A, C\n");
        add8(_cpu->A, _cpu->C);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x82: {
        // printf("ADD A, D\n");
        add8(_cpu->A, _cpu->D);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x83: {
        // printf("ADD A, E\n");
        add8(_cpu->A, _cpu->E);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x84: {
        // printf("ADD A, H\n");
        add8(_cpu->A, _cpu->H);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x85: {
        // printf("ADD A, L\n");
        add8(_cpu->A, _cpu->L);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x86: {
        // printf("ADD A, (HL)\n");
        uint8_t value = _bus->read8(_cpu->HL);
        add8_mem(_cpu->A, value);
        _cpu->PC += 1;
        break;
    }
    case 0x87: {
        // printf("ADD A, A\n");
        add8(_cpu->A, _cpu->A);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x88: {
        // printf("ADD A, B\n");
        adc(_cpu->A, _cpu->B);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x89: {
        // printf("ADC A, C\n");
        adc(_cpu->A, _cpu->C);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x8A: {
        // printf("ADC A, D\n");
        adc(_cpu->A, _cpu->D);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x8B: {
        // printf("ADC A, E\n");
        adc(_cpu->A, _cpu->E);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x8C: {
        // printf("ADC A, H\n");
        adc(_cpu->A, _cpu->H);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x8D: {
        // printf("ADC A, L\n");
        adc(_cpu->A, _cpu->L);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x8E: {
        // printf("ADC A, (HL)\n");
        adc(_cpu->A, _bus->read8(_cpu->HL));
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x8F: {
        // printf("ADC A, L\n");
        adc(_cpu->A, _cpu->A);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x90: {
        // printf("SUB A, L\n");
        sub(_cpu->A, _cpu->B);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x91: {
        // printf("SUB C\n");
        sub(_cpu->A, _cpu->C);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x92: {
        // printf("SUB D\n");
        sub(_cpu->A, _cpu->D);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x93: {
        // printf("SUB E\n");
        sub(_cpu->A, _cpu->E);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x94: {
        // printf("SUB H\n");
        sub(_cpu->A, _cpu->H);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x95: {
        // printf("SUB L\n");
        sub(_cpu->A, _cpu->L);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x96: {
        // printf("SUB (HL)\n");
        sub(_cpu->A, _bus->read8(_cpu->HL));
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x97: {
        // printf("SUB A\n");
        sub(_cpu->A, _cpu->A);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x98: {
        // printf("SBC A, B\n");
        sbc(_cpu->A, _cpu->B);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x99: {
        // printf("SBC A, C\n");
        sbc(_cpu->A, _cpu->C);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x9A: {
        // printf("SBC A, D\n");
        sbc(_cpu->A, _cpu->D);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x9B: {
        // printf("SBC A, E\n");
        sbc(_cpu->A, _cpu->E);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x9C: {
        // printf("SBC A, H\n");
        sbc(_cpu->A, _cpu->H);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x9D: {
        // printf("SBC A, L\n");
        sbc(_cpu->A, _cpu->L);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x9E: {
        // printf("SBC A, L\n");
        sbc(_cpu->A, _bus->read8(_cpu->HL));
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0x9F: {
        // printf("SBC A, A\n");
        sbc(_cpu->A, _cpu->A);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xA0: {
        // printf("AND A, B\n");
        and_(_cpu->A, _cpu->B);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xA1: {
        // printf("AND A, C\n");
        and_(_cpu->A, _cpu->C);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xA2: {
        // printf("AND A, C\n");
        and_(_cpu->A, _cpu->D);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xA3: {
        // printf("AND A, E\n");
        and_(_cpu->A, _cpu->E);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xA4: {
        // printf("AND A, H\n");
        and_(_cpu->A, _cpu->H);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xA5: {
        // printf("AND A, L\n");
        and_(_cpu->A, _cpu->L);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xA6: {
        // printf("AND A, (HL)\n");
        and_(_cpu->A, _bus->read8(_cpu->HL));
        _cpu->PC = _cpu->PC + 1;
        break;
    }

    case 0xA7: {
        // printf("AND A, (HL)\n");
        and_(_cpu->A, _cpu->A);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xA8: {
        // printf("XOR A, B\n");
        xor_(_cpu->A, _cpu->B);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xA9: {
        // printf("XOR A, C\n");
        xor_(_cpu->A, _cpu->C);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xAA: {
        // printf("XOR A, D\n");
        xor_(_cpu->A, _cpu->D);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xAB: {
        // printf("XOR A, E\n");
        xor_(_cpu->A, _cpu->E);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xAC: {
        // printf("XOR A, H\n");
        xor_(_cpu->A, _cpu->H);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xAD: {
        // printf("XOR A, L\n");
        xor_(_cpu->A, _cpu->L);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xAE: {
        // printf("XOR A, (HL)\n");
        xor_(_cpu->A, _bus->read8(_cpu->HL));
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xAF: {
        // printf("XOR A, A\n");
        xor_(_cpu->A, _cpu->A);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xB0: {
        // printf("OR A, B\n");
        or_(_cpu->A, _cpu->B);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xB1: {
        // printf("OR A, C\n");
        or_(_cpu->A, _cpu->C);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xB2: {
        // printf("OR A, D\n");
        or_(_cpu->A, _cpu->D);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xB3: {
        // printf("OR A, E\n");
        or_(_cpu->A, _cpu->E);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xB4: {
        // printf("OR A, H\n");
        or_(_cpu->A, _cpu->H);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xB5: {
        // printf("OR A, L\n");
        or_(_cpu->A, _cpu->L);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xB6: {
        // printf("OR A, (HL)\n");
        or_(_cpu->A, _bus->read8(_cpu->HL));
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xB7: {
        // printf("OR A, A\n");
        or_(_cpu->A, _cpu->A);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xB8: {
        // printf("CP B\n");
        cp_(_cpu->A, _cpu->B);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xB9: {
        // printf("CP C\n");
        cp_(_cpu->A, _cpu->C);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xBA: {
        // printf("CP D\n");
        cp_(_cpu->A, _cpu->D);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xBB: {
        // printf("CP E\n");
        cp_(_cpu->A, _cpu->E);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xBC: {
        // printf("CP H\n");
        cp_(_cpu->A, _cpu->H);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xBD: {
        // printf("CP L\n");
        cp_(_cpu->A, _cpu->L);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xBE: {
        // printf("CP (HL)\n");
        cp_(_cpu->A, _bus->read8(_cpu->HL));
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xBF: {
        // printf("CP A\n");
        cp_(_cpu->A, _cpu->A);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xC0: {
        // printf("RET NZ\n");
        if (!(_cpu->F & FLAG_ZERO)) {
            ret(true);
        } else {
            _cpu->PC = _cpu->PC + 1;
        }
        break;
    }
    case 0xC1: {
        // printf("POP BC\n");
        uint8_t l = _bus->read8(_cpu->SP);
        _cpu->SP += 1;
        uint8_t h = _bus->read8(_cpu->SP);
        _cpu->SP += 1;

        _cpu->BC = (h << 8) | l;
        _cpu->PC += 1;
        break;
    }
    case 0xC2: {
        // printf("JP NZ, nn\n");
        if (!(_cpu->F & FLAG_ZERO)) {
            uint8_t l = _bus->read8(_cpu->PC + 1);
            uint8_t h = _bus->read8(_cpu->PC + 2);
            _cpu->PC  = (h << 8) | l;
        } else {
            _cpu->PC += 3;
        }
        break;
    }
    case 0xC3: {
        uint8_t l = _bus->read8(_cpu->PC + 1);
        uint8_t h = _bus->read8(_cpu->PC + 2);
        _cpu->PC  = (h << 8) | l;
        // printf("JP a16 -- %X --\n", _cpu->PC);
        break;
    }
    case 0xC4: {
        // printf("CALL nn 0xc4");
        call(!(_cpu->F & FLAG_ZERO));
        break;
    }
    case 0xC5: {
        // printf("PUSH BC\n");
        push_(_cpu->BC);
        _cpu->PC += 1;
        break;
    }
    case 0xC6: {
        // printf("ADD A, d8\n");
        add8(_cpu->A, _bus->read8(_cpu->PC + 1));
        _cpu->PC = _cpu->PC + 2;
        break;
    }
    case 0xC7: {
        // printf("RST 0\n");
        uint16_t ret = _cpu->PC + 1;
        _cpu->SP -= 1;
        _bus->write8(_cpu->SP, (ret >> 8) & 0xFF);
        _cpu->SP -= 1;
        _bus->write8(_cpu->SP, ret & 0xFF);
        _cpu->PC = _cpu->PC + 1;
        _cpu->PC = 0x0000;
        break;
    }
    case 0xC8: {
        // printf("RET Z\n");
        if (_cpu->F & FLAG_ZERO) {
            ret(true);
        } else {
            _cpu->PC += 1;
        }
        break;
    }
    case 0xC9: {
        // printf("RET\n");
        pop_(true, _cpu->PC);
        break;
    }
    case 0xCA: {
        // printf("JP Z, nn 0xCA\n");
        if (_cpu->F & FLAG_ZERO) {
            u_int8_t l = _bus->read8(_cpu->PC + 1);
            u_int8_t h = _bus->read8(_cpu->PC + 2);
            _cpu->PC   = (h << 8) | l;
        } else {
            _cpu->PC += 3;
        }
        break;
    }
    case 0xCB: {
        uint8_t cb_op = _bus->read8(_cpu->PC + 1);
        switch (cb_op) {
        case 0x00: {
            // printf("RLC B\n");
            rlc_extended(_cpu->B);
            break;
        }
        case 0x01: {
            // printf("RLC c\n");
            rlc_extended(_cpu->C);

            break;
        }
        case 0x02: {
            // printf("rlc d\n");
            rlc_extended(_cpu->D);

            break;
        }
        case 0x03: {
            // printf("RLC E\n");
            rlc_extended(_cpu->E);

            break;
        }
        case 0x04: {
            // printf("RLC H\n");
            rlc_extended(_cpu->H);

            break;
        }
        case 0x05: {
            // printf("RLC L\n");
            rlc_extended(_cpu->L);

            break;
        }
        case 0x06: {
            // printf("RLC (HL)\n");
            uint8_t v = _bus->read8(_cpu->HL);
            rlc_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0x07: {
            // printf("RLC A\n");
            rlc_extended(_cpu->A);

            break;
        }
        case 0x08: {
            // printf("RRC B\n");
            rrc_extended(_cpu->B);

            break;
        }
        case 0x09: {
            // printf("RRC C\n");
            rrc_extended(_cpu->C);

            break;
        }
        case 0x0A: {
            // printf("RRC D\n");
            rrc_extended(_cpu->D);

            break;
        }
        case 0x0B: {
            // printf("RRC E\n");
            rrc_extended(_cpu->E);

            break;
        }
        case 0x0C: {
            // printf("RRC H\n");
            rrc_extended(_cpu->H);

            break;
        }
        case 0x0D: {
            // printf("RRC L\n");
            rrc_extended(_cpu->L);

            break;
        }
        case 0x0E: {
            // printf("RRC (HL)\n");
            uint8_t v = _bus->read8(_cpu->HL);
            rrc_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0x0F: {
            // printf("RL B\n");
            rrc_extended(_cpu->A);

            break;
        }
        case 0x10: {
            // printf("RL B \n");
            rl_extended(_cpu->B);
            break;
        }
        case 0x11: {
            // printf("RL C\n");
            rl_extended(_cpu->C);

            break;
        }
        case 0x12: {
            // printf("RL D\n");
            rl_extended(_cpu->D);

            break;
        }
        case 0x13: {
            // printf("RL E\n");
            rl_extended(_cpu->E);

            break;
        }
        case 0x14: {
            // printf("RL H\n");
            rl_extended(_cpu->H);

            break;
        }
        case 0x15: {
            // printf("RL L \n");
            rl_extended(_cpu->L);

            break;
        }
        case 0x16: {
            // printf("RL (HL)");
            uint8_t v = _bus->read8(_cpu->HL);
            rl_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0x17: {
            // printf("RL A\n");
            rl_extended(_cpu->A);

            break;
        }
        case 0x18: {
            // printf("RR B\n");
            rr_extended(_cpu->B);

            break;
        }
        case 0x19: {
            // printf("RR B\n");
            rr_extended(_cpu->C);

            break;
        }
        case 0x1A: {
            // printf("RR D\n");
            rr_extended(_cpu->D);

            break;
        }
        case 0x1B: {
            // printf("RR D\n");
            rr_extended(_cpu->E);

            break;
        }
        case 0x1C: {
            // printf("RR H\n");
            rr_extended(_cpu->H);

            break;
        }
        case 0x1D: {
            // printf("RR L\n");
            rr_extended(_cpu->L);

            break;
        }
        case 0x1E: {
            // printf("RR L\n");
            uint8_t v = _bus->read8(_cpu->HL);
            rr_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0x1F: {
            // printf("RR A\n");
            rr_extended(_cpu->A);

            break;
        }
        case 0x20: {
            // printf("SLA B\n");
            sla_extended(_cpu->B);

            break;
        }
        case 0x21: {
            // printf("SLA C\n");
            sla_extended(_cpu->C);

            break;
        }
        case 0x22: {
            // printf("SLA D\n");
            sla_extended(_cpu->D);

            break;
        }
        case 0x23: {
            // printf("SLA E\n");
            sla_extended(_cpu->E);

            break;
        }
        case 0x24: {
            // printf("SLA H\n");
            sla_extended(_cpu->H);

            break;
        }
        case 0x25: {
            // printf("SLA L\n");
            sla_extended(_cpu->L);

            break;
        }
        case 0x26: {
            // printf("SLA (HL)\n");
            uint8_t v = _bus->read8(_cpu->HL);
            sla_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0x27: {
            // printf("SLA A\n");
            sla_extended(_cpu->A);

            break;
        }
        case 0x28: {
            // printf("SLA B\n");
            sra_extended(_cpu->B);

            break;
        }
        case 0x29: {
            // printf("SRA C\n");
            sra_extended(_cpu->C);

            break;
        }
        case 0x2A: {
            // printf("SRA C\n");
            sra_extended(_cpu->D);

            break;
        }
        case 0x2B: {
            // printf("SRA D\n");
            sra_extended(_cpu->E);

            break;
        }
        case 0x2C: {
            // printf("SRA E\n");
            sra_extended(_cpu->H);

            break;
        }
        case 0x2D: {
            // printf("SRA L\n");
            sra_extended(_cpu->L);

            break;
        }
        case 0x2E: {
            // printf("SRA (HL)\n");
            uint8_t v = _bus->read8(_cpu->HL);
            sra_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0x2F: {
            // printf("SRA A\n");
            sra_extended(_cpu->A);

            break;
        }
        case 0x30: {
            // printf("SWAP B\n");
            swap_extended(_cpu->B);

            break;
        }
        case 0x31: {
            // printf("SWAP C\n");
            swap_extended(_cpu->C);

            break;
        }
        case 0x32: {
            // printf("SWAP D\n");
            swap_extended(_cpu->D);

            break;
        }
        case 0x33: {
            // printf("SWAP E\n");
            swap_extended(_cpu->E);

            break;
        }
        case 0x34: {
            // printf("SWAP H\n");
            swap_extended(_cpu->H);

            break;
        }
        case 0x35: {
            // printf("SWAP L\n");
            swap_extended(_cpu->L);

            break;
        }
        case 0x36: {
            // printf("SWAP (HL)\n");
            uint8_t v = _bus->read8(_cpu->HL);
            swap_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0x37: {
            // printf("SWAP A\n");
            swap_extended(_cpu->A);

            break;
        }
        case 0x38: {
            // printf("SRL B\n");
            srl_extended(_cpu->B);

            break;
        }
        case 0x39: {
            // printf("SRL C\n");
            srl_extended(_cpu->C);

            break;
        }
        case 0x3A: {
            // printf("SRL D\n");
            srl_extended(_cpu->D);

            break;
        }
        case 0x3B: {
            // printf("SRL E\n");
            srl_extended(_cpu->E);

            break;
        }
        case 0x3C: {
            // printf("SRL H\n");
            srl_extended(_cpu->H);

            break;
        }
        case 0x3D: {
            // printf("SRL L\n");
            srl_extended(_cpu->L);

            break;
        }
        case 0x3E: {
            // printf("SRL (HL)\n");
            uint8_t v = _bus->read8(_cpu->HL);
            srl_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0x3F: {
            // printf("SRL A\n");
            srl_extended(_cpu->A);

            break;
        }
        case 0x40: {
            // printf("BIT0 B\n");
            bit0_extended(_cpu->B);

            break;
        }
        case 0x41: {
            // printf("BIT0 C\n");
            bit0_extended(_cpu->C);

            break;
        }
        case 0x42: {
            // printf("BIT0 D\n");
            bit0_extended(_cpu->D);

            break;
        }
        case 0x43: {
            // printf("BIT0 E\n");
            bit0_extended(_cpu->E);

            break;
        }
        case 0x44: {
            // printf("BIT0 H\n");
            bit0_extended(_cpu->H);

            break;
        }
        case 0x45: {
            // printf("BIT0 L\n");
            bit0_extended(_cpu->L);

            break;
        }
        case 0x46: {
            // printf("BIT0 (HL)\n");
            uint8_t v = _bus->read8(_cpu->HL);
            bit0_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0x47: {
            // printf("BIT0 A\n");
            bit0_extended(_cpu->A);

            break;
        }
        case 0x48: {
            // printf("BIT1 B\n");
            bit1_extended(_cpu->B);

            break;
        }
        case 0x49: {
            // printf("BIT1 C\n");
            bit1_extended(_cpu->C);

            break;
        }
        case 0x4A: {
            // printf("BIT1 D\n");
            bit1_extended(_cpu->D);

            break;
        }
        case 0x4B: {
            // printf("BIT1 E\n");
            bit1_extended(_cpu->E);

            break;
        }
        case 0x4C: {
            // printf("BIT1 H\n");
            bit1_extended(_cpu->H);

            break;
        }
        case 0x4D: {
            // printf("BIT1 L\n");
            bit1_extended(_cpu->L);

            break;
        }
        case 0x4E: {
            uint8_t v = _bus->read8(_cpu->HL);
            // printf("BIT1 (HL)\n");
            bit1_extended(v);
            _bus->write8(_cpu->HL, v);
            break;
        }
        case 0x4F: {
            // printf("BIT1 A\n");
            bit1_extended(_cpu->A);

            break;
        }
        case 0x50: {
            // printf("BIT2, B\n");
            bit2_extended(_cpu->B);

            break;
        }
        case 0x51: {
            // printf("BIT2, C\n");
            bit2_extended(_cpu->C);

            break;
        }
        case 0x52: {
            // printf("BIT2, D\n");
            bit2_extended(_cpu->D);

            break;
        }
        case 0x53: {
            // printf("BIT2, E\n");
            bit2_extended(_cpu->E);

            break;
        }
        case 0x54: {
            // printf("BIT2, H\n");
            bit2_extended(_cpu->H);

            break;
        }
        case 0x55: {
            // printf("BIT2, L\n");
            bit2_extended(_cpu->L);

            break;
        }
        case 0x56: {
            // printf("BIT2, L\n");
            uint8_t v = _bus->read8(_cpu->HL);
            bit2_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0x57: {
            // printf("BIT2 A\n");
            bit2_extended(_cpu->A);

            break;
        }
        case 0x58: {
            // printf("BIT3 B\n");
            bit3_extended(_cpu->B);

            break;
        }
        case 0x59: {
            // printf("BIT3 C\n");
            bit3_extended(_cpu->C);

            break;
        }
        case 0x5A: {
            // printf("BIT3 D\n");
            bit3_extended(_cpu->D);

            break;
        }
        case 0x5B: {
            // printf("BIT3 E\n");
            bit3_extended(_cpu->E);

            break;
        }
        case 0x5C: {
            // printf("BIT3 H\n");
            bit3_extended(_cpu->H);

            break;
        }
        case 0x5D: {
            // printf("BIT3 L\n");
            bit3_extended(_cpu->L);

            break;
        }
        case 0x5E: {
            // printf("BIT3 L\n");
            uint8_t v = _bus->read8(_cpu->HL);
            bit3_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0x5F: {
            // printf("BIT3 A\n");
            bit3_extended(_cpu->A);

            break;
        }
        case 0x60: {
            // printf("BIT4 B\n");
            bit4_extended(_cpu->B);

            break;
        }
        case 0x61: {
            // printf("BIT4 C\n");
            bit4_extended(_cpu->C);

            break;
        }
        case 0x62: {
            // printf("BIT4 D\n");
            bit4_extended(_cpu->D);
            break;
        }
        case 0x63: {
            // printf("BIT4 E\n");
            bit4_extended(_cpu->E);

            break;
        }
        case 0x64: {
            // printf("BIT4 H\n");
            bit4_extended(_cpu->H);

            break;
        }
        case 0x65: {
            // printf("BIT4 L\n");
            bit4_extended(_cpu->L);

            break;
        }
        case 0x66: {
            // printf("BIT4 L\n");
            uint8_t v = _bus->read8(_cpu->HL);
            bit4_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0x67: {
            // printf("BIT4 A\n");
            bit4_extended(_cpu->A);

            break;
        }
        case 0x68: {
            // printf("BIT4 A\n");
            bit5_extended(_cpu->B);
            break;
        }
        case 0x69: {
            // printf("BIT5 B\n");
            bit5_extended(_cpu->C);

            break;
        }
        case 0x6A: {
            // printf("BIT5 C\n");
            bit5_extended(_cpu->D);

            break;
        }
        case 0x6B: {
            // printf("BIT5 D\n");
            bit5_extended(_cpu->E);

            break;
        }
        case 0x6C: {
            // printf("BIT5 E\n");
            bit5_extended(_cpu->H);

            break;
        }
        case 0x6D: {
            // printf("BIT5 L\n");
            bit5_extended(_cpu->L);

            break;
        }
        case 0x6E: {
            uint8_t v = _bus->read8(_cpu->HL);
            bit5_extended(v);
            _bus->write8(_cpu->HL, v);
            break;
        }
        case 0x6F: {
            bit5_extended(_cpu->A);
            break;
        }
        case 0x70: {
            // printf("BIT6 A\n");
            bit6_extended(_cpu->B);
            break;
        }
        case 0x71: {
            // printf("BIT6 C\n");
            bit6_extended(_cpu->C);

            break;
        }
        case 0x72: {
            // printf("BIT6 D\n");
            bit6_extended(_cpu->D);

            break;
        }
        case 0x73: {
            // printf("BIT6 E\n");
            bit6_extended(_cpu->E);

            break;
        }
        case 0x74: {
            // printf("BIT6 H\n");
            bit6_extended(_cpu->H);

            break;
        }
        case 0x75: {
            // printf("BIT6 L\n");
            bit6_extended(_cpu->L);

            break;
        }
        case 0x76: {
            // printf("BIT6 L\n");
            uint8_t v = _bus->read8(_cpu->HL);
            bit6_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0x77: {
            // printf("BIT6 A\n");
            bit6_extended(_cpu->A);

            break;
        }
        case 0x78: {
            // printf("BIT7 A\n");
            bit7_extended(_cpu->B);

            break;
        }
        case 0x79: {
            // printf("BIT7 C\n");
            bit7_extended(_cpu->C);

            break;
        }
        case 0x7A: {
            // printf("BIT7 D\n");
            bit7_extended(_cpu->D);

            break;
        }
        case 0x7B: {
            // printf("BIT7 E\n");
            bit7_extended(_cpu->E);

            break;
        }
        case 0x7C: {
            // printf("BIT7 H\n");
            bit7_extended(_cpu->H);

            break;
        }
        case 0x7D: {
            // printf("BIT7 L\n");
            bit7_extended(_cpu->L);

            break;
        }
        case 0x7E: {
            // printf("BIT7 L\n");
            uint8_t v = _bus->read8(_cpu->HL);
            bit7_extended(v);
            _bus->write8(_cpu->HL, v);
            break;
        }
        case 0x7F: {
            // printf("BIT7 A\n");
            bit7_extended(_cpu->A);

            break;
        }
        case 0x80: {
            // printf("RES0 B\n");
            res0_extended(_cpu->B);

            break;
        }
        case 0x81: {
            // printf("RES0 C\n");
            res0_extended(_cpu->C);

            break;
        }
        case 0x82: {
            // printf("RES0 D\n");
            res0_extended(_cpu->D);

            break;
        }
        case 0x83: {
            // printf("RES0 E\n");
            res0_extended(_cpu->E);

            break;
        }
        case 0x84: {
            // printf("RES0 H\n");
            res0_extended(_cpu->H);

            break;
        }
        case 0x85: {
            // printf("RES0 L\n");
            res0_extended(_cpu->L);

            break;
        }
        case 0x86: {
            // printf("RES0 (HL)\n");
            uint8_t v = _bus->read8(_cpu->HL);
            res0_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0x87: {
            // printf("RES0 L\n");
            res0_extended(_cpu->A);

            break;
        }
        case 0x88: {
            // printf("RES1 B\n");
            res1_extended(_cpu->B);

            break;
        }
        case 0x89: {
            // printf("RES1 C\n");
            res1_extended(_cpu->C);

            break;
        }
        case 0x8A: {
            // printf("RES1 D\n");
            res1_extended(_cpu->D);

            break;
        }
        case 0x8B: {
            // printf("RES1 E\n");
            res1_extended(_cpu->E);

            break;
        }
        case 0x8C: {
            // printf("RES1 E\n");
            res1_extended(_cpu->H);

            break;
        }
        case 0x8D: {
            // printf("RES1 L\n");
            res1_extended(_cpu->L);

            break;
        }
        case 0x8E: {
            // printf("RES1 (HL)\n");
            uint8_t v = _bus->read8(_cpu->HL);
            res1_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0x8F: {
            // printf("RES1 A\n");
            res1_extended(_cpu->A);

            break;
        }
        case 0x90: {
            // printf("RES2 B\n");
            res2_extended(_cpu->B);

            break;
        }
        case 0x91: {
            // printf("RES2 C\n");
            res2_extended(_cpu->C);

            break;
        }
        case 0x92: {
            // printf("RES2 D\n");
            res2_extended(_cpu->D);

            break;
        }
        case 0x93: {
            // printf("RES2 E\n");
            res2_extended(_cpu->E);

            break;
        }
        case 0x94: {
            // printf("RES2 H\n");
            res2_extended(_cpu->H);

            break;
        }
        case 0x95: {
            // printf("RES2 L\n");
            res2_extended(_cpu->L);

            break;
        }

        case 0x96: {
            // printf("RES2 L\n");
            uint8_t v = _bus->read8(_cpu->HL);
            res2_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0x97: {
            // printf("RES2 A\n");
            res2_extended(_cpu->A);

            break;
        }
        case 0x98: {
            // printf("RES3 B\n");
            res3_extended(_cpu->B);

            break;
        }
        case 0x99: {
            // printf("RES3 C\n");
            res3_extended(_cpu->C);

            break;
        }
        case 0x9A: {
            // printf("RES3 D\n");
            res3_extended(_cpu->D);

            break;
        }
        case 0x9B: {
            // printf("RES3 E\n");
            res3_extended(_cpu->E);

            break;
        }
        case 0x9C: {
            // printf("RES3 H\n");
            res3_extended(_cpu->H);

            break;
        }
        case 0x9D: {
            // printf("RES3 L\n");
            res3_extended(_cpu->L);

            break;
        }
        case 0x9E: {
            // printf("RES3 L\n");
            uint8_t v = _bus->read8(_cpu->HL);
            res3_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0x9F: {
            // printf("RES3 A\n");
            res3_extended(_cpu->A);

            break;
        }
        case 0xA0: {
            // printf("RES4 A\n");
            res4_extended(_cpu->B);
            break;
        }
        case 0xA1: {
            // printf("RES4 C\n");
            res4_extended(_cpu->C);

            break;
        }
        case 0xA2: {
            // printf("RES4 D\n");
            res4_extended(_cpu->D);

            break;
        }
        case 0xA3: {
            // printf("RES4 E\n");
            res4_extended(_cpu->E);

            break;
        }
        case 0xA4: {
            // printf("RES4 H\n");
            res4_extended(_cpu->H);

            break;
        }
        case 0xA5: {
            // printf("RES4 L\n");
            res4_extended(_cpu->L);

            break;
        }
        case 0xA6: {
            // printf("RES4 L\n");
            uint8_t v = _bus->read8(_cpu->HL);
            res4_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0xA7: {
            // printf("RES4 A\n");
            res4_extended(_cpu->A);

            break;
        }
        case 0xA8: {
            // printf("RES5 B\n");
            res5_extended(_cpu->B);

            break;
        }
        case 0xA9: {
            // printf("RES5 C\n");
            res5_extended(_cpu->C);

            break;
        }
        case 0xAA: {
            // printf("RES5 D\n");
            res5_extended(_cpu->D);

            break;
        }
        case 0xAB: {
            // printf("RES5 E\n");
            res5_extended(_cpu->E);

            break;
        }
        case 0xAC: {
            // printf("RES5 H\n");
            res5_extended(_cpu->H);

            break;
        }
        case 0xAD: {
            // printf("RES5 L\n");
            res5_extended(_cpu->L);

            break;
        }
        case 0xAE: {
            // printf("RES5 (HL)\n");
            uint8_t v = _bus->read8(_cpu->HL);
            res5_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0xAF: {
            // printf("RES5 A\n");
            res5_extended(_cpu->A);

            break;
        }
        case 0xB0: {
            // printf("RES6 B\n");
            res6_extended(_cpu->B);

            break;
        }
        case 0xB1: {
            // printf("RES6 C\n");
            res6_extended(_cpu->C);

            break;
        }
        case 0xB2: {
            // printf("RES6 D\n");
            res6_extended(_cpu->D);

            break;
        }
        case 0xB3: {
            // printf("RES6 E\n");
            res6_extended(_cpu->E);

            break;
        }
        case 0xB4: {
            // printf("RES6 H\n");
            res6_extended(_cpu->H);

            break;
        }
        case 0xB5: {
            // printf("RES6 L\n");
            res6_extended(_cpu->L);

            break;
        }
        case 0xB6: {
            // printf("RES6 (HL)\n");
            uint8_t v = _bus->read8(_cpu->HL);
            res6_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0xB7: {
            // printf("RES6 A\n");
            res6_extended(_cpu->A);

            break;
        }
        case 0xB8: {
            // printf("RES7 B\n");
            res7_extended(_cpu->B);

            break;
        }
        case 0xB9: {
            // printf("RES7 C\n");
            res7_extended(_cpu->C);

            break;
        }
        case 0xBA: {
            // printf("RES7 D\n");
            res7_extended(_cpu->D);

            break;
        }
        case 0xBB: {
            // printf("RES7 E\n");
            res7_extended(_cpu->E);

            break;
        }
        case 0xBC: {
            // printf("RES7 H\n");
            res7_extended(_cpu->H);

            break;
        }
        case 0xBD: {
            // printf("RES7 L\n");
            res7_extended(_cpu->L);

            break;
        }
        case 0xBE: {
            // printf("RES7 (HL)\n");
            uint8_t v = _bus->read8(_cpu->HL);
            res7_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0xBF: {
            // printf("RES7 A\n");
            res7_extended(_cpu->A);

            break;
        }
        case 0xC0: {
            // printf("SET0 B\n");
            set0_extended(_cpu->B);

            break;
        }
        case 0xC1: {
            // printf("SET0 C\n");
            set0_extended(_cpu->C);

            break;
        }
        case 0xC2: {
            // printf("SET0 D\n");
            set0_extended(_cpu->D);

            break;
        }
        case 0xC3: {
            // printf("SET0 E\n");
            set0_extended(_cpu->E);

            break;
        }
        case 0xC4: {
            // printf("SET0 H\n");
            set0_extended(_cpu->H);

            break;
        }
        case 0xC5: {
            // printf("SET0 L\n");
            set0_extended(_cpu->L);

            break;
        }
        case 0xC6: {
            // printf("SET0 L\n");
            uint8_t v = _bus->read8(_cpu->HL);
            set0_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0xC7: {
            // printf("SET0 A\n");
            set0_extended(_cpu->A);

            break;
        }
        case 0xC8: {
            // printf("SET1 B\n");
            set1_extended(_cpu->B);

            break;
        }
        case 0xC9: {
            // printf("SET1 C\n");
            set1_extended(_cpu->C);

            break;
        }
        case 0xCA: {
            // printf("SET1 D\n");
            set1_extended(_cpu->D);

            break;
        }
        case 0xCB: {
            // printf("SET1 E\n");
            set1_extended(_cpu->E);

            break;
        }
        case 0xCC: {
            // printf("SET1 H\n");
            set1_extended(_cpu->H);

            break;
        }
        case 0xCD: {
            // printf("SET1 L\n");
            set1_extended(_cpu->L);

            break;
        }
        case 0xCE: {
            // printf("SET1 (HL)\n");
            uint8_t v = _bus->read8(_cpu->HL);
            set1_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0xCF: {
            // printf("SET1 A\n");
            set1_extended(_cpu->A);

            break;
        }
        case 0xD0: {
            // printf("SET2 B\n");
            set2_extended(_cpu->B);

            break;
        }
        case 0xD1: {
            // printf("SET2 C\n");
            set2_extended(_cpu->C);

            break;
        }
        case 0xD2: {
            // printf("SET2 D\n");
            set2_extended(_cpu->D);

            break;
        }
        case 0xD3: {
            // printf("SET2 E\n");
            set2_extended(_cpu->E);

            break;
        }
        case 0xD4: {
            // printf("SET2 D\n");
            set2_extended(_cpu->H);
            break;
        }
        case 0xD5: {
            // printf("SET2 L\n");
            set2_extended(_cpu->L);

            break;
        }
        case 0xD6: {
            // printf("SET2 L\n");
            uint8_t v = _bus->read8(_cpu->HL);
            set2_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0xD7: {
            // printf("SET2 A\n");
            set2_extended(_cpu->A);

            break;
        }
        case 0xD8: {
            // printf("SET3 B\n");
            set3_extended(_cpu->B);

            break;
        }
        case 0xD9: {
            // printf("SET3 C\n");
            set3_extended(_cpu->C);

            break;
        }
        case 0xDA: {
            // printf("SET3 D\n");
            set3_extended(_cpu->D);

            break;
        }
        case 0xDB: {
            // printf("SET3 E\n");
            set3_extended(_cpu->E);

            break;
        }
        case 0xDC: {
            // printf("SET3 H\n");
            set3_extended(_cpu->H);

            break;
        }
        case 0xDD: {
            // printf("SET3 L\n");
            set3_extended(_cpu->L);

            break;
        }
        case 0xDE: {
            // printf("SET3 (HL)\n");
            uint8_t v = _bus->read8(_cpu->HL);
            set3_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0xDF: {
            // printf("SET3 A\n");
            set3_extended(_cpu->A);

            break;
        }
        case 0xE0: {
            // printf("SET4 B\n");
            set4_extended(_cpu->B);

            break;
        }
        case 0xE1: {
            // printf("SET4 C\n");
            set4_extended(_cpu->C);

            break;
        }
        case 0xE2: {
            // printf("SET4 D\n");
            set4_extended(_cpu->D);

            break;
        }
        case 0xE3: {
            // printf("SET4 E\n");
            set4_extended(_cpu->E);

            break;
        }
        case 0xE4: {
            // printf("SET4 H\n");
            set4_extended(_cpu->H);

            break;
        }
        case 0xE5: {
            // printf("SET4 L\n");
            set4_extended(_cpu->L);

            break;
        }
        case 0xE6: {
            // printf("SET4 (HL)\n");
            uint8_t v = _bus->read8(_cpu->HL);
            set4_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0xE7: {
            // printf("SET4 A\n");
            set4_extended(_cpu->A);

            break;
        }
        case 0xE8: {
            // printf("SET5 B\n");
            set5_extended(_cpu->B);

            break;
        }
        case 0xE9: {
            // printf("SET5 C\n");
            set5_extended(_cpu->C);

            break;
        }
        case 0xEA: {
            // printf("SET5 D\n");
            set5_extended(_cpu->D);

            break;
        }
        case 0xEB: {
            // printf("SET5 E\n");
            set5_extended(_cpu->E);

            break;
        }
        case 0xEC: {
            // printf("SET5 H\n");
            set5_extended(_cpu->H);

            break;
        }
        case 0xED: {
            // printf("SET5 L\n");
            set5_extended(_cpu->L);

            break;
        }
        case 0xEE: {
            // printf("SET5 (HL)\n");
            uint8_t v = _bus->read8(_cpu->HL);
            set5_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0xEF: {
            // printf("SET5 A\n");
            set5_extended(_cpu->A);

            break;
        }
        case 0xF0: {
            // printf("SET6 B\n");
            set6_extended(_cpu->B);

            break;
        }
        case 0xF1: {
            // printf("SET6 C\n");
            set6_extended(_cpu->C);

            break;
        }
        case 0xF2: {
            // printf("SET6 D\n");
            set6_extended(_cpu->D);

            break;
        }
        case 0xF3: {
            // printf("SET6 E\n");
            set6_extended(_cpu->E);

            break;
        }
        case 0xF4: {
            // printf("SET6 H\n");
            set6_extended(_cpu->H);

            break;
        }
        case 0xF5: {
            // printf("SET6 L\n");
            set6_extended(_cpu->L);

            break;
        }
        case 0xF6: {
            // printf("SET6 (HL)\n");
            uint8_t v = _bus->read8(_cpu->HL);
            set6_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0xF7: {
            // printf("SET6 A\n");
            set6_extended(_cpu->A);

            break;
        }
        case 0xF8: {
            // printf("SET7 B\n");
            set7_extended(_cpu->B);

            break;
        }
        case 0xF9: {
            // printf("SET7 C\n");
            set7_extended(_cpu->C);

            break;
        }
        case 0xFA: {
            // printf("SET7 D\n");
            set7_extended(_cpu->D);

            break;
        }
        case 0xFB: {
            // printf("SET7 E\n");
            set7_extended(_cpu->E);

            break;
        }
        case 0xFC: {
            // printf("SET7 H\n");
            set7_extended(_cpu->H);

            break;
        }
        case 0xFD: {
            // printf("SET7 L\n");
            set7_extended(_cpu->L);

            break;
        }
        case 0xFE: {
            // printf("SET7 L\n");
            uint8_t v = _bus->read8(_cpu->HL);
            set7_extended(v);
            _bus->write8(_cpu->HL, v);

            break;
        }
        case 0xFF: {
            // printf("SET7 A\n");
            set7_extended(_cpu->A);

            break;
        }
        default: {
            printf("-- not CB prefix with that value %02X --\n", _bus->read8(_cpu->PC + 1));
            exit(1);
            break;
        }
        }
        _cpu->PC += 2;
        break;
    }
    case 0xCC: {
        // printf("CALL Z, a16\n");
        if (_cpu->F & FLAG_ZERO) {
            execute_call();
        } else {
            _cpu->PC = _cpu->PC + 3;
        }
        break;
    }
    case 0xCD: {
        // printf("CALL a16\n");
        uint16_t address        = _bus->read16(_cpu->PC + 1);
        uint16_t return_address = _cpu->PC + 3;

        _cpu->SP -= 2;
        _bus->write16(_cpu->SP, return_address);

        _cpu->PC = address;
        break;
    }
    case 0xCE: {
        adc(_cpu->A, _bus->read8(_cpu->PC + 1));
        _cpu->PC = _cpu->PC + 2;
        break;
    }
    case 0xCF: {
        // printf("RST 1\n");
        uint16_t return_addr = _cpu->PC + 1;
        _cpu->SP -= 2;
        _bus->write16(_cpu->SP, return_addr);
        _cpu->PC = 0x0008;
        break;
    }
    case 0xD0: {
        // printf("RET NC\n");
        if (!(_cpu->F & FLAG_CARRY)) {
            pop_(true, _cpu->PC);
        } else {
            _cpu->PC += 1;
        }
        break;
    }
    case 0xD1: {
        // printf("POP DE\n");
        pop_(true, _cpu->DE);
        _cpu->PC += 1;
        break;
    }
    case 0xD2: {
        if (!(_cpu->F & FLAG_CARRY)) {
            uint8_t l = _bus->read8(_cpu->PC + 1);
            uint8_t h = _bus->read8(_cpu->PC + 2);
            _cpu->PC  = (h << 8) | l;
        } else {
            _cpu->PC += 3;
        }
        break;
    }
    case 0xD3: {
        // printf("-- NOT IMPLEMENTED --\n");
        //  _cpu->PC = _cpu->PC + 2;
        break;
    }
    case 0xD4: {
        // printf("  CALL NC, a16\n");
        call(!(_cpu->F & FLAG_CARRY));
        break;
    }
    case 0xD5: {
        // printf(" PUSH DE\n");
        push_(_cpu->DE);
        _cpu->PC += 1;
        break;
    }
    case 0xD6: {
        sub(_cpu->A, _bus->read8(_cpu->PC + 1));
        // printf("SUB d8. result of A -- %X -- 0xD6\n", _cpu->A);
        _cpu->PC = _cpu->PC + 2;
        break;
    }
    case 0xD7: {
        // printf("RST 10H\n");
        uint16_t ret = _cpu->PC + 1;
        _cpu->SP--;
        _bus->write8(_cpu->SP, (ret >> 8) & 0xFF);
        _cpu->SP--;
        _bus->write8(_cpu->SP, ret & 0xFF);
        _cpu->PC = 0x0010;
        break;
    }
    case 0xD8: {
        if (_cpu->F & FLAG_CARRY) {
            ret(true);
        } else {
            _cpu->PC += 1;
        }
        return;
    }
    case 0xD9: {
        // printf("-- 0xD9 -- \n");
        pop_(true, _cpu->PC);
        _cpu->_ime = 1;
        // _cpu->PC += 1;
        break;
    }
    case 0xDA: {
        // printf("JP C, nn\n");
        if (_cpu->F & FLAG_CARRY) {
            uint8_t l = _bus->read8(_cpu->PC + 1);
            uint8_t h = _bus->read8(_cpu->PC + 2);
            _cpu->PC  = (h << 8) | l;
        } else {
            _cpu->PC = _cpu->PC + 3;
        }
        break;
    }
    case 0xDB: {
        // printf("-- NOT IMPLEMENTED --\n");
        //  _cpu->PC = _cpu->PC + 2;
        break;
    }

    case 0xDC: {
        // printf("CALL Z, a16\n");
        if (_cpu->F & FLAG_CARRY) {
            execute_call();
        } else {
            _cpu->PC += 3;
        }
        break;
    }
    case 0xDD: {
        // printf("-- NOT IMPLEMENTED --\n");
        //  _cpu->PC = _cpu->PC + 2;
        break;
    }
    case 0xDE: {
        // printf("SBC A, d8\n");
        sbc(_cpu->A, _bus->read8(_cpu->PC + 1));
        _cpu->PC = _cpu->PC + 2;
        break;
    }
    case 0xDF: {
        // printf("RST 18H\n");
        uint16_t return_addr = _cpu->PC + 1;

        _cpu->SP -= 1;
        _bus->write8(_cpu->SP, (return_addr >> 8) & 0xFF);

        _cpu->SP -= 1;
        _bus->write8(_cpu->SP, return_addr & 0xFF);

        _cpu->PC = 0x0018;
        break;
    }
    case 0xE0: {
        uint8_t a8 = _bus->read8(_cpu->PC + 1);

        _bus->write8(0xFF00 + a8, _cpu->A);

        _cpu->PC += 2;
        break;
    }
    case 0xE1: {
        // printf("POP HL\n");
        pop_(true, _cpu->HL);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xE2: {
        // uint8_t a8 = _bus->read8(_cpu->PC + 1);
        _bus->write8(0xff00 + _cpu->C, _cpu->A);
        _cpu->PC += 1;
        break;
    }
    case 0xE3: {
        // printf("-- NOT IMPLEMENTED --\n");
        break;
    }
    case 0xE4: {
        // printf("-- NOT IMPLEMENTED --\n");
        break;
    }
    case 0xE5: {
        // printf("PUSH HL\n");
        push_(_cpu->HL);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xE6: {
        // printf(" AND d8\n");
        and_(_cpu->A, _bus->read8(_cpu->PC + 1));
        _cpu->PC = _cpu->PC + 2;
        break;
    }
    case 0xE7: {
        // printf("RST 4\n");
        uint16_t n8 = _cpu->PC + 1;
        push_(n8);
        _cpu->PC = 0x0020;
        break;
    }
    case 0xE8: {
        // printf("ADD SP, r8\n");
        int8_t s8 = (int8_t)_bus->read8(_cpu->PC + 1);
        _cpu->clear_flag(FLAG_ZERO);
        _cpu->clear_flag(FLAG_SUBTRACT);
        uint16_t old_sp = _cpu->SP;
        uint16_t result = old_sp + s8;
        _cpu->set_flag(FLAG_HALF_CARRY, ((old_sp & 0xF) + (s8 & 0xF)) > 0xF);
        _cpu->set_flag(FLAG_CARRY, ((old_sp & 0xFF) + (s8 & 0xFF)) > 0xFF);
        _cpu->SP = result;

        _cpu->PC = _cpu->PC + 2;
        break;
    }
    case 0xE9: {
        // printf("JP (HL)\n");
        _cpu->PC = _cpu->HL;
        break;
    }
    case 0xEA: {
        uint8_t  l  = _bus->read8(_cpu->PC + 1);
        uint8_t  h  = _bus->read8(_cpu->PC + 2);
        uint16_t nn = (h << 8) | l;
        // printf("LD(nn), A 0xEA nn: -- %X --\n", nn);
        _bus->write8(nn, _cpu->A);
        _cpu->PC = _cpu->PC + 3;
        break;
    }
    case 0xEB: {
        // printf("-- NOT IMPLEMENTED --\n");
        //  _cpu->PC = _cpu->PC + 3;
        break;
    }
    case 0xEC: {
        // printf("-- NOT IMPLEMENTED --\n");
        //  _cpu->PC = _cpu->PC + 3;
        break;
    }
    case 0xED: {
        // printf("-- NOT IMPLEMENTED --\n");
        //  _cpu->PC = _cpu->PC + 3;
        break;
    }
    case 0xEE: {
        // printf(" XOR d8\n");
        xor_(_cpu->A, _bus->read8(_cpu->PC + 1));
        _cpu->PC = _cpu->PC + 2;
        break;
    }
    case 0xEF: {
        // printf(" RST 28H\n");
        uint16_t return_addr = _cpu->PC + 1;
        push_(return_addr);
        _cpu->PC = 0x0028;
        break;
    }
    case 0xF0: {
        // printf("LD A, (a8)\n");
        uint8_t  a8   = _bus->read8(_cpu->PC + 1);
        uint16_t addr = 0xFF00 | a8;
        _cpu->A       = _bus->read8(addr);

        _cpu->PC += 2;
        break;
    }
    case 0xF1: {
        // printf("POP AF\n");
        pop_(true, _cpu->AF);
        _cpu->AF &= 0xFFF0;
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xF2: {
        // printf("LDA, (C)\n");
        _cpu->A  = _bus->read8(0xFF00 + _cpu->C);
        _cpu->PC = _cpu->PC + 1;
        break;
    }
    case 0xF3: {
        // printf(" DI\n");
        _cpu->_ime        = 0;
        _cpu->ime_pending = false;
        _cpu->PC += 1;
    }
    case 0xF4: {
        // printf("-- NOT IMPLEMENTED --\n");
        //  _cpu->PC = _cpu->PC + 3;
        break;
    }
    case 0xF5: {
        // printf("push AF\n");
        //  _cpu->PC = _cpu->PC + 3;
        push_(_cpu->AF);
        _cpu->PC += 1;
        break;
    }
    case 0xF6: {
        // printf(" OR nn\n");
        or_(_cpu->A, _bus->read8(_cpu->PC + 1));
        _cpu->PC = _cpu->PC + 2;
        break;
    }
    case 0xF7: {
        // printf(" RST 30H\n");
        uint16_t n8 = _cpu->PC + 1;
        push_(n8);
        _cpu->PC = 0x0030;
        break;
    }
    case 0xF8: {
        // printf("LD HL, SP+s8\n");
        int8_t   s8 = _bus->read8(_cpu->PC + 1);
        uint32_t r  = _cpu->SP + s8;
        _cpu->PC    = _cpu->PC + 2;
        _cpu->HL    = r;
        _cpu->clear_flag(FLAG_ZERO);
        _cpu->clear_flag(FLAG_SUBTRACT);
        _cpu->set_flag(FLAG_HALF_CARRY, (_cpu->SP & 0x0F) + (s8 & 0x0F) > 0x0F);
        _cpu->set_flag(FLAG_CARRY, ((_cpu->SP & 0xFF) + (s8 & 0xFF)) > 0xFF);
        break;
    }
    case 0xF9: {
        // printf("  LDSP, HL\n");
        _cpu->SP = _cpu->HL;
        _cpu->PC += 1;
        break;
    }
    case 0xFA: {
        // printf("LD A, (a16)\n");
        uint8_t  l = _bus->read8(_cpu->PC + 1);
        uint8_t  h = _bus->read8(_cpu->PC + 2);
        uint16_t a = (h << 8) | l;
        _cpu->A    = _bus->read8(a);
        _cpu->PC   = _cpu->PC + 3;
        break;
    }
    case 0xFB: {
        printf("EI\n");
        _cpu->ime_pending = true;
        _cpu->PC += 1;
        break;
    }
    case 0xFC: {
        // printf("-- NOT IMPLEMENTED --\n");
        //  _cpu->PC = _cpu->PC + 3;
        break;
    }
    case 0xFD: {
        // printf("-- NOT IMPLEMENTED --\n");
        //  _cpu->PC = _cpu->PC + 3;
        break;
    }
    case 0xFE: {
        uint8_t n = _bus->read8(_cpu->PC + 1);
        // printf("CP A,n8  -- %X --\n", n);
        _cpu->set_flag(FLAG_ZERO, n == _cpu->A);
        _cpu->set_flag(FLAG_SUBTRACT, 1);
        _cpu->set_flag(FLAG_HALF_CARRY, (_cpu->A & 0x0F) < (n & 0x0F));
        _cpu->set_flag(FLAG_CARRY, _cpu->A < n);
        _cpu->PC = _cpu->PC + 2;
        break;
    }
    case 0xFF: {
        // printf("RST 38H");
        uint16_t return_addr = _cpu->PC + 1;
        push_(return_addr);
        _cpu->PC = 0x0038;
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
        uint8_t l = _bus->read8(_cpu->SP);
        _cpu->SP += 1;
        uint8_t h = _bus->read8(_cpu->SP);
        _cpu->SP += 1;
        reg = (h << 8) | l;
    }
}

void InstructionSet::ret(bool condition) {
    if (condition) {
        uint8_t l = _bus->read8(_cpu->SP);
        _cpu->SP += 1;
        uint8_t h = _bus->read8(_cpu->SP);
        _cpu->SP += 1;
        _cpu->PC = (h << 8) | l;
    }
}

void InstructionSet::or_(uint8_t& reg_1, uint8_t reg_2) {
    uint8_t tmp = reg_1 | reg_2;

    _cpu->F = 0;
    _cpu->set_flag(FLAG_ZERO, (tmp == 0));
    reg_1 = tmp;
}

void InstructionSet::cp_(uint8_t reg_1, uint8_t reg_2) {
    uint16_t tmp = reg_1 - reg_2;
    _cpu->set_flag(FLAG_ZERO, tmp == 0);
    _cpu->set_flag(FLAG_SUBTRACT, true);
    _cpu->set_flag(FLAG_HALF_CARRY, (reg_1 & 0x0F) < (reg_2 & 0x0F));
    _cpu->set_flag(FLAG_CARRY, tmp > 0xFF);
}

void InstructionSet::call(bool condition) {
    if (condition) {
        uint8_t  l   = _bus->read8(_cpu->PC + 1);
        uint8_t  h   = _bus->read8(_cpu->PC + 2);
        uint16_t t   = (h << 8) | l;
        uint16_t ret = _cpu->PC += 3;

        _cpu->SP -= 1;
        _bus->write8(_cpu->SP, ret >> 8);
        _cpu->SP -= 1;
        _bus->write8(_cpu->SP, ret & 0xFF);
        _cpu->PC = t;
    } else {
        _cpu->PC += 3;
    }
}

void InstructionSet::and_(uint8_t& reg_1, uint8_t reg_2) {
    reg_1 = reg_1 & reg_2;
    _cpu->set_flag(FLAG_ZERO, reg_1 == 0);
    _cpu->set_flag(FLAG_HALF_CARRY, 1);
    _cpu->clear_flag(FLAG_SUBTRACT);
    _cpu->clear_flag(FLAG_CARRY);
}

void InstructionSet::sbc(uint8_t& reg_1, uint8_t reg_2) {
    uint8_t  old    = reg_1;
    uint8_t  a      = reg_1;
    uint8_t  b      = reg_2;
    bool     c      = (_cpu->F & FLAG_CARRY);
    uint16_t result = a - b - c;

    reg_1 = result & 0xFF;
    _cpu->set_flag(FLAG_ZERO, (reg_1 == 0));
    _cpu->set_flag(FLAG_SUBTRACT, true);
    _cpu->set_flag(FLAG_HALF_CARRY, (old & 0x0F) < (b & 0x0F) + c);
    _cpu->set_flag(FLAG_CARRY, old < (reg_2 + c));
}
void InstructionSet::execute_call() {
    // address being called
    uint8_t l = _bus->read8(_cpu->PC + 1);
    uint8_t h = _bus->read8(_cpu->PC + 2);

    // next instruction after call
    uint16_t ret_addr = _cpu->PC + 3;

    _cpu->SP -= 1;
    _bus->write8(_cpu->SP, (ret_addr >> 8) & 0xFF);
    _cpu->SP -= 1;
    _bus->write8(_cpu->SP, ret_addr & 0xFF);
    _cpu->PC = (h << 8) | l;
}

void InstructionSet::xor_(uint8_t& reg_1, uint8_t reg_2) {
    reg_1 ^= reg_2;

    _cpu->set_flag(FLAG_ZERO, reg_1 == 0);
    _cpu->set_flag(FLAG_HALF_CARRY, false);
    _cpu->set_flag(FLAG_CARRY, false);
    _cpu->set_flag(FLAG_SUBTRACT, false);
}

void InstructionSet::push_(uint16_t reg) {
    _cpu->SP -= 1;
    uint8_t h = reg >> 8;
    _bus->write8(_cpu->SP, h);
    _cpu->SP -= 1;
    uint8_t l = reg & 0xFF;
    _bus->write8(_cpu->SP, l);
}

// void InstructionSet::cpl(uint8_t &reg) {
//   // DONE
//   reg = ~reg;
//
//   cp_(uint8_t * reg_1, uint8_t * reg_2) {
//     uint16_t tmp = reg_1 - reg_2;
//     _cpu->set_flag(FLAG_ZERO, (reg_1 == reg_2));
//     _cpu->set_flag(FLAG_SUBTRACT, 1);
//     _cpu->set_flag(FLAG_HALF_CARRY, ((*reg_1 & 0x0F) < (*reg_2 & 0x0F)));
//     _cpu->set_flag(FLAG_CARRY, (tmp > 0xFF));
//   }
// }
void InstructionSet::rlc(uint8_t reg) {
    bool msb = reg & 0x80;
    reg      = (reg << 1) | (msb >> 7);

    _cpu->set_flag(FLAG_CARRY, msb);

    _cpu->set_flag(FLAG_ZERO, reg == 0);

    _cpu->clear_flag(FLAG_SUBTRACT);
    _cpu->clear_flag(FLAG_HALF_CARRY);
}

void InstructionSet::rrca(uint8_t& reg) {
    // DONE
    bool least_sig_bit = reg & 1;
    reg                = reg >> 1;
    if (least_sig_bit) {
        reg |= 128;
    }
    _cpu->set_flag(FLAG_CARRY, least_sig_bit);
    _cpu->clear_flag(FLAG_ZERO);
    _cpu->clear_flag(FLAG_SUBTRACT);
    _cpu->clear_flag(FLAG_HALF_CARRY);
}

// void and_(uint8_t reg_1, uint8_t reg_2) {
//     reg_1 = reg_1 & reg_2;
//     _cpu->set_flag(FLAG_ZERO, reg_1 == 0);
//     _cpu->clear_flag(FLAG_SUBTRACT);
//     _cpu->set_flag(FLAG_HALF_CARRY, 1);
//     _cpu->clear_flag(FLAG_CARRY);
// }

void InstructionSet::dec_mem(uint16_t& reg) {
    // To be DONE
    uint8_t tmp          = _bus->read8(reg);
    uint8_t nibble_carry = tmp & 0x0F;
    _cpu->set_flag(FLAG_HALF_CARRY, (tmp & 0x0F) == 0);
    tmp = tmp - 1;
    _bus->write8(reg, tmp);
    _cpu->set_flag(FLAG_ZERO, tmp == 0);
    _cpu->set_flag(FLAG_SUBTRACT, 1);
}

void InstructionSet::adc(uint8_t& reg_1, uint8_t reg_2) {
    bool old_c = _cpu->F & FLAG_CARRY;

    uint16_t c = reg_1 + reg_2 + old_c;
    _cpu->set_flag(FLAG_ZERO, (c & 0xFF) == 0);
    _cpu->clear_flag(FLAG_SUBTRACT);
    _cpu->set_flag(FLAG_HALF_CARRY, ((reg_1 & 0x0F) + (reg_2 & 0x0F) + old_c) > 0x0F);
    _cpu->set_flag(FLAG_CARRY, c > 0xFF);
    reg_1 = c;
}

void InstructionSet::rra() {
    // DONE
    // Before: C = c, A = 76543210
    // After: A = c7654321, C = 0
    bool c  = _cpu->F & FLAG_CARRY;
    bool b0 = _cpu->A & 1;

    _cpu->A = _cpu->A >> 1;
    if (c) {
        _cpu->A |= 0x80;
    }
    _cpu->clear_flag(FLAG_ZERO);
    _cpu->clear_flag(FLAG_SUBTRACT);
    _cpu->clear_flag(FLAG_HALF_CARRY);
    _cpu->set_flag(FLAG_CARRY, b0);
}

void InstructionSet::rla() {
    printf("RLA\n");
    uint8_t old_a     = _cpu->A;
    uint8_t old_carry = _cpu->F & FLAG_CARRY;
    uint8_t new_carry = (old_a >> 7) & 1;
    _cpu->A           = (old_a << 1) | old_carry;

    _cpu->set_flag(FLAG_CARRY, new_carry);
    _cpu->clear_flag(FLAG_ZERO);
    _cpu->clear_flag(FLAG_SUBTRACT);
    _cpu->clear_flag(FLAG_HALF_CARRY);
}
void InstructionSet::add8_mem(uint8_t& destination, uint8_t value) {
    // _bus->write8(destination, destination + value);
    uint16_t v = destination + value;
    _cpu->set_flag(FLAG_ZERO, (v & 0xFF) == 0);
    _cpu->set_flag(FLAG_SUBTRACT, false);
    _cpu->set_flag(FLAG_HALF_CARRY, ((destination & 0x0F) + (value & 0x0F)) > 0x0F);
    _cpu->set_flag(FLAG_CARRY, v > 0xFF);
    destination = v;
}
void InstructionSet::add8(uint8_t& reg_1, uint8_t reg_2) {
    // DONE:
    uint8_t  result = reg_1 + reg_2;
    uint16_t _carry = reg_1 + reg_2;
    _cpu->set_flag(FLAG_ZERO, result == 0);
    _cpu->set_flag(FLAG_SUBTRACT, false);
    _cpu->set_flag(FLAG_HALF_CARRY, (reg_1 & 0x0F) + (reg_2 & 0x0F) >= 0x10);
    _cpu->set_flag(FLAG_CARRY, _carry >= 0x0100);
    reg_1 = result;
}
void InstructionSet::sub(uint8_t& reg_1, uint8_t reg_2) {
    uint8_t a = reg_1;
    uint8_t b = reg_2;

    uint16_t result = a - b;

    _cpu->set_flag(FLAG_ZERO, ((result & 0xFF) == 0));
    _cpu->set_flag(FLAG_SUBTRACT, true);

    _cpu->set_flag(FLAG_HALF_CARRY, (a & 0x0F) < (b & 0x0F));
    _cpu->set_flag(FLAG_CARRY, a < b);
    reg_1 = a - b;
}
void InstructionSet::add16(uint16_t& destination, uint16_t& value) {
    // DONE
    uint32_t result = destination + value;
    _cpu->clear_flag(FLAG_SUBTRACT);
    _cpu->set_flag(FLAG_CARRY, result > 0xFFFF);
    _cpu->set_flag(FLAG_HALF_CARRY, ((destination & 0x0FFF) + (value & 0x0FFF)) > 0x0FFF);
    destination = result & 0xFFFF;
}

void InstructionSet::inc(uint8_t& reg) {
    // DONE
    uint8_t old = reg;

    reg = old + 1;

    _cpu->set_flag(FLAG_ZERO, reg == 0);
    _cpu->clear_flag(FLAG_SUBTRACT);
    _cpu->set_flag(FLAG_HALF_CARRY, (old & 0x0F) == 0x0F);
}

void InstructionSet::dec(uint8_t& reg) {
    // DONE

    uint8_t r = reg - 1;
    _cpu->set_flag(FLAG_ZERO, r == 0);
    _cpu->set_flag(FLAG_SUBTRACT, true);
    _cpu->set_flag(FLAG_HALF_CARRY, (reg & 0x0F) == 0x00);
    reg = r;
}

void InstructionSet::inc_mem(uint16_t& reg) {
    // @brief this function increments
    // the value at the [reg],
    // and updates the F flag register
    // accordinly.
    uint8_t tmp          = _bus->read8(reg);
    uint8_t nibble_carry = tmp & 0x0F;
    tmp                  = tmp + 1;
    _bus->write8(reg, tmp);

    _cpu->set_flag(FLAG_HALF_CARRY, (nibble_carry == 0x0F));
    _cpu->set_flag(FLAG_ZERO, (tmp == 0));
    _cpu->clear_flag(FLAG_SUBTRACT);
}

void InstructionSet::cpl(uint8_t& reg) {
    // DONE
    reg = ~reg;

    _cpu->set_flag(FLAG_SUBTRACT, true);
    _cpu->set_flag(FLAG_HALF_CARRY, true);
}

uint8_t _opcode;

void InstructionSet::step() {
    if (_cpu->ime_pending) {
        _cpu->ime_pending = false;
        _cpu->_ime        = 1;
    }
    _opcode = _bus->read8(_cpu->PC);
    execute(_opcode);
    int current_cycle = _cpu->opcode_cycles[_opcode];
    _cpu->cycle_count += current_cycle;
}

void InstructionSet::interrupt_handler(uint16_t vector) {
    _cpu->_ime  = 0;
    uint16_t pc = _cpu->PC;
    _cpu->SP -= 1;
    _bus->write8(_cpu->SP, (pc >> 8) & 0xFF);
    _cpu->SP -= 1;
    _bus->write8(_cpu->SP, pc & 0xFF);
    _cpu->PC = vector;
    _cpu->cycle_count += 20;
}
