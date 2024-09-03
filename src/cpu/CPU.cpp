#include <cstdint>
#include <fstream>
#include <iostream>

#include "CPU.hpp"

CPU::CPU() {
    init_opcode_table();
}

void CPU::init_opcode_table() {

    // Initialize the opcode table
    opcode_table[0x00] = [this]() { nop(); };                  // 0x00: NOP
    opcode_table[0x01] = [this]() { load_bc_nn(); };           // 0x01: LD BC, nn
    opcode_table[0x02] = [this]() { load_bc_a(); };            // 0x02: LD (BC), A
    opcode_table[0x03] = [this]() { inc_bc(); };               // 0x03: INC BC
    opcode_table[0x04] = [this]() { inc_b(); };                // 0x04: INC B
    opcode_table[0x05] = [this]() { dec_b(); };                // 0x05: DEC B
    opcode_table[0x06] = [this]() { load_b_n(); };             // 0x06: LD B, n
    opcode_table[0x07] = [this]() { rlca(); };                 // 0x07: RLCA
    opcode_table[0x08] = [this]() { load_nn_sp(); };           // 0x08: LD (nn), SP
    opcode_table[0x09] = [this]() { add_hl_bc(); };            // 0x09: ADD HL, BC
    opcode_table[0x0A] = [this]() { load_a_bc(); };            // 0x0A: LD A, (BC)
    opcode_table[0x0B] = [this]() { dec_bc(); };               // 0x0B: DEC BC
    opcode_table[0x0C] = [this]() { inc_c(); };                // 0x0C: INC C
    opcode_table[0x0D] = [this]() { dec_c(); };                // 0x0D: DEC C
    opcode_table[0x0E] = [this]() { load_c_n(); };             // 0x0E: LD C, n
    opcode_table[0x0F] = [this]() { rrca(); };                 // 0x0F: RRCA

    opcode_table[0x10] = [this]() { stop(); };                 // 0x10: STOP
    opcode_table[0x11] = [this]() { load_de_nn(); };           // 0x11: LD DE, nn
    opcode_table[0x12] = [this]() { load_de_a(); };            // 0x12: LD (DE), A
    opcode_table[0x13] = [this]() { inc_de(); };               // 0x13: INC DE
    opcode_table[0x14] = [this]() { inc_d(); };                // 0x14: INC D
    opcode_table[0x15] = [this]() { dec_d(); };                // 0x15: DEC D
    opcode_table[0x16] = [this]() { load_d_n(); };             // 0x16: LD D, n
    opcode_table[0x17] = [this]() { rla(); };                  // 0x17: RLA
    opcode_table[0x18] = [this]() { jr_n(); };                 // 0x18: JR n
    opcode_table[0x19] = [this]() { add_hl_de(); };            // 0x19: ADD HL, DE
    opcode_table[0x1A] = [this]() { load_a_de(); };            // 0x1A: LD A, (DE)
    opcode_table[0x1B] = [this]() { dec_de(); };               // 0x1B: DEC DE
    opcode_table[0x1C] = [this]() { inc_e(); };                // 0x1C: INC E
    opcode_table[0x1D] = [this]() { dec_e(); };                // 0x1D: DEC E
    opcode_table[0x1E] = [this]() { load_e_n(); };             // 0x1E: LD E, n
    opcode_table[0x1F] = [this]() { rra(); };                  // 0x1F: RRA
                                                               //
    opcode_table[0x20] = [this]() { jr_nz_n(); };              // 0x20: JR NZ, n
    opcode_table[0x21] = [this]() { load_hl_nn(); };           // 0x21: LD HL, nn
    opcode_table[0x22] = [this]() { load_hl_plus_a(); };       // 0x22: LD (HL+), A
    opcode_table[0x23] = [this]() { inc_hl(); };               // 0x23: INC HL
    opcode_table[0x24] = [this]() { inc_h(); };                // 0x24: INC H
    opcode_table[0x25] = [this]() { dec_h(); };                // 0x25: DEC H
    opcode_table[0x26] = [this]() { load_h_n(); };             // 0x26: LD H, n
    opcode_table[0x27] = [this]() { daa(); };                  // 0x27: DAA
    opcode_table[0x28] = [this]() { jr_z_n(); };               // 0x28: JR Z, n
    opcode_table[0x29] = [this]() { add_hl_hl(); };            // 0x29: ADD HL, HL
    opcode_table[0x2A] = [this]() { load_a_hl_plus(); };       // 0x2A: LD A, (HL+)
    opcode_table[0x2B] = [this]() { dec_hl(); };               // 0x2B: DEC HL
    opcode_table[0x2C] = [this]() { inc_l(); };                // 0x2C: INC L
    opcode_table[0x2D] = [this]() { dec_l(); };                // 0x2D: DEC L
    opcode_table[0x2E] = [this]() { load_l_n(); };             // 0x2E: LD L, n
    opcode_table[0x2F] = [this]() { cpl(); };                  // 0x2F: CPL

    opcode_table[0x30] = [this]() { jr_nc_n(); };           // 0x30: JR NC, n
    opcode_table[0x31] = [this]() { load_sp_nn(); };        // 0x31: LD SP, nn
    opcode_table[0x32] = [this]() { load_hl_minus_a(); };   // 0x32: LD (HL-), A
    opcode_table[0x33] = [this]() { inc_sp(); };            // 0x33: INC SP
    opcode_table[0x34] = [this]() { inc_hl_mem(); };        // 0x34: INC (HL)
    opcode_table[0x35] = [this]() { dec_hl_mem(); };        // 0x35: DEC (HL)
    opcode_table[0x36] = [this]() { load_hl_n(); };         // 0x36: LD (HL), n
    opcode_table[0x37] = [this]() { scf(); };               // 0x37: SCF
    opcode_table[0x38] = [this]() { jr_c_n(); };            // 0x38: JR C, n
    opcode_table[0x39] = [this]() { add_hl_sp(); };         // 0x39: ADD HL, SP
    opcode_table[0x3A] = [this]() { load_a_hl_minus(); };   // 0x3A: LD A, (HL-)
    opcode_table[0x3B] = [this]() { dec_sp(); };            // 0x3B: DEC SP
    opcode_table[0x3C] = [this]() { inc_a(); };             // 0x3C: INC A
    opcode_table[0x3D] = [this]() { dec_a(); };             // 0x3D: DEC A
    opcode_table[0x3E] = [this]() { load_a_n(); };          // 0x3E: LD A, n
    opcode_table[0x3F] = [this]() { ccf(); };               // 0x3F: CCF

    opcode_table[0x40] = [this]() { load_b_b(); };         // 0x40: LD B, B
    opcode_table[0x41] = [this]() { load_b_c(); };         // 0x41: LD B, C
    opcode_table[0x42] = [this]() { load_b_d(); };         // 0x42: LD B, D
    opcode_table[0x43] = [this]() { load_b_e(); };         // 0x43: LD B, E
    opcode_table[0x44] = [this]() { load_b_h(); };         // 0x44: LD B, H
    opcode_table[0x45] = [this]() { load_b_l(); };         // 0x45: LD B, L
    opcode_table[0x46] = [this]() { load_b_hl_mem(); };    // 0x46: LD B, (HL)
    opcode_table[0x47] = [this]() { load_b_a(); };         // 0x47: LD B, A
    opcode_table[0x48] = [this]() { load_c_b(); };         // 0x48: LD C, B
    opcode_table[0x49] = [this]() { load_c_c(); };         // 0x49: LD C, C
    opcode_table[0x4A] = [this]() { load_c_d(); };         // 0x4A: LD C, D
    opcode_table[0x4B] = [this]() { load_c_e(); };         // 0x4B: LD C, E
    opcode_table[0x4C] = [this]() { load_c_h(); };         // 0x4C: LD C, H
    opcode_table[0x4D] = [this]() { load_c_l(); };         // 0x4D: LD C, L
    opcode_table[0x4E] = [this]() { load_c_hl_mem(); };    // 0x4E: LD C, (HL)
    opcode_table[0x4F] = [this]() { load_c_a(); };         // 0x4F: LD C, A

    opcode_table[0x50] = [this]() { load_d_b(); };         // 0x50: LD D, B
    opcode_table[0x51] = [this]() { load_d_c(); };         // 0x51: LD D, C
    opcode_table[0x52] = [this]() { load_d_d(); };         // 0x52: LD D, D
    opcode_table[0x53] = [this]() { load_d_e(); };         // 0x53: LD D, E
    opcode_table[0x54] = [this]() { load_d_h(); };         // 0x54: LD D, H
    opcode_table[0x55] = [this]() { load_d_l(); };         // 0x55: LD D, L
    opcode_table[0x56] = [this]() { load_d_hl_mem(); };    // 0x56: LD D, (HL)
    opcode_table[0x57] = [this]() { load_d_a(); };         // 0x57: LD D, A
    opcode_table[0x58] = [this]() { load_e_b(); };         // 0x58: LD E, B
    opcode_table[0x59] = [this]() { load_e_c(); };         // 0x59: LD E, C
    opcode_table[0x5A] = [this]() { load_e_d(); };         // 0x5A: LD E, D
    opcode_table[0x5B] = [this]() { load_e_e(); };         // 0x5B: LD E, E
    opcode_table[0x5C] = [this]() { load_e_h(); };         // 0x5C: LD E, H
    opcode_table[0x5D] = [this]() { load_e_l(); };         // 0x5D: LD E, L
    opcode_table[0x5E] = [this]() { load_e_hl_mem(); };    // 0x5E: LD E, (HL)
    opcode_table[0x5F] = [this]() { load_e_a(); };         // 0x5F: LD E, A

    opcode_table[0x60] = [this]() { load_h_b(); };         // 0x60: LD H, B
    opcode_table[0x61] = [this]() { load_h_c(); };         // 0x61: LD H, C
    opcode_table[0x62] = [this]() { load_h_d(); };         // 0x62: LD H, D
    opcode_table[0x63] = [this]() { load_h_e(); };         // 0x63: LD H, E
    opcode_table[0x64] = [this]() { load_h_h(); };         // 0x64: LD H, H
    opcode_table[0x65] = [this]() { load_h_l(); };         // 0x65: LD H, L
    opcode_table[0x66] = [this]() { load_h_hl_mem(); };    // 0x66: LD H, (HL)
    opcode_table[0x67] = [this]() { load_h_a(); };         // 0x67: LD H, A
    opcode_table[0x68] = [this]() { load_l_b(); };         // 0x68: LD L, B
    opcode_table[0x69] = [this]() { load_l_c(); };         // 0x69: LD L, C
    opcode_table[0x6A] = [this]() { load_l_d(); };         // 0x6A: LD L, D
    opcode_table[0x6B] = [this]() { load_l_e(); };         // 0x6B: LD L, E
    opcode_table[0x6C] = [this]() { load_l_h(); };         // 0x6C: LD L, H
    opcode_table[0x6D] = [this]() { load_l_l(); };         // 0x6D: LD L, L
    opcode_table[0x6E] = [this]() { load_l_hl_mem(); };    // 0x6E: LD L, (HL)
    opcode_table[0x6F] = [this]() { load_l_a(); };         // 0x6F: LD L, A

    opcode_table[0x70] = [this]() { load_hl_mem_b(); };    // 0x70: LD (HL), B
    opcode_table[0x71] = [this]() { load_hl_mem_c(); };    // 0x71: LD (HL), C
    opcode_table[0x72] = [this]() { load_hl_mem_d(); };    // 0x72: LD (HL), D
    opcode_table[0x73] = [this]() { load_hl_mem_e(); };    // 0x73: LD (HL), E
    opcode_table[0x74] = [this]() { load_hl_mem_h(); };    // 0x74: LD (HL), H
    opcode_table[0x75] = [this]() { load_hl_mem_l(); };    // 0x75: LD (HL), L
    opcode_table[0x76] = [this]() { halt(); };             // 0x76: HALT
    opcode_table[0x77] = [this]() { load_hl_mem_a(); };    // 0x77: LD (HL), A
    opcode_table[0x78] = [this]() { load_a_b(); };         // 0x78: LD A, B
    opcode_table[0x79] = [this]() { load_a_c(); };         // 0x79: LD A, C
    opcode_table[0x7A] = [this]() { load_a_d(); };         // 0x7A: LD A, D
    opcode_table[0x7B] = [this]() { load_a_e(); };         // 0x7B: LD A, E
    opcode_table[0x7C] = [this]() { load_a_h(); };         // 0x7C: LD A, H
    opcode_table[0x7D] = [this]() { load_a_l(); };         // 0x7D: LD A, L
    opcode_table[0x7E] = [this]() { load_a_hl_mem(); };    // 0x7E: LD A, (HL)
    opcode_table[0x7F] = [this]() { load_a_a(); };         // 0x7F: LD A, A

    opcode_table[0x80] = [this]() { add_a_b(); };          // 0x80: ADD A, B
    opcode_table[0x81] = [this]() { add_a_c(); };          // 0x81: ADD A, C
    opcode_table[0x82] = [this]() { add_a_d(); };          // 0x82: ADD A, D
    opcode_table[0x83] = [this]() { add_a_e(); };          // 0x83: ADD A, E
    opcode_table[0x84] = [this]() { add_a_h(); };          // 0x84: ADD A, H
    opcode_table[0x85] = [this]() { add_a_l(); };          // 0x85: ADD A, L
    opcode_table[0x86] = [this]() { add_a_hl_mem(); };     // 0x86: ADD A, (HL)
    opcode_table[0x87] = [this]() { add_a_a(); };          // 0x87: ADD A, A
    opcode_table[0x88] = [this]() { adc_a_b(); };          // 0x88: ADC A, B
    opcode_table[0x89] = [this]() { adc_a_c(); };          // 0x89: ADC A, C
    opcode_table[0x8A] = [this]() { adc_a_d(); };          // 0x8A: ADC A, D
    opcode_table[0x8B] = [this]() { adc_a_e(); };          // 0x8B: ADC A, E
    opcode_table[0x8C] = [this]() { adc_a_h(); };          // 0x8C: ADC A, H
    opcode_table[0x8D] = [this]() { adc_a_l(); };          // 0x8D: ADC A, L
    opcode_table[0x8E] = [this]() { adc_a_hl_mem(); };     // 0x8E: ADC A, (HL)
    opcode_table[0x8F] = [this]() { adc_a_a(); };          // 0x8F: ADC A, A                                                               //

    opcode_table[0x90] = [this]() { sub_a_b(); };          // 0x90: SUB B
    opcode_table[0x91] = [this]() { sub_a_c(); };          // 0x91: SUB C
    opcode_table[0x92] = [this]() { sub_a_d(); };          // 0x92: SUB D
    opcode_table[0x93] = [this]() { sub_a_e(); };          // 0x93: SUB E
    opcode_table[0x94] = [this]() { sub_a_h(); };          // 0x94: SUB H
    opcode_table[0x95] = [this]() { sub_a_l(); };          // 0x95: SUB L
    opcode_table[0x96] = [this]() { sub_a_hl_mem(); };     // 0x96: SUB (HL)
    opcode_table[0x97] = [this]() { sub_a_a(); };          // 0x97: SUB A
    opcode_table[0x98] = [this]() { sbc_a_b(); };          // 0x98: SBC A, B
    opcode_table[0x99] = [this]() { sbc_a_c(); };          // 0x99: SBC A, C
    opcode_table[0x9A] = [this]() { sbc_a_d(); };          // 0x9A: SBC A, D
    opcode_table[0x9B] = [this]() { sbc_a_e(); };          // 0x9B: SBC A, E
    opcode_table[0x9C] = [this]() { sbc_a_h(); };          // 0x9C: SBC A, H
    opcode_table[0x9D] = [this]() { sbc_a_l(); };          // 0x9D: SBC A, L
    opcode_table[0x9E] = [this]() { sbc_a_hl_mem(); };     // 0x9E: SBC A, (HL)
    opcode_table[0x9F] = [this]() { sbc_a_a(); };          // 0x9F: SBC A, A

    opcode_table[0xA0] = [this]() { and_a_b(); };          // 0xA0: AND B
    opcode_table[0xA1] = [this]() { and_a_c(); };          // 0xA1: AND C
    opcode_table[0xA2] = [this]() { and_a_d(); };          // 0xA2: AND D
    opcode_table[0xA3] = [this]() { and_a_e(); };          // 0xA3: AND E
    opcode_table[0xA4] = [this]() { and_a_h(); };          // 0xA4: AND H
    opcode_table[0xA5] = [this]() { and_a_l(); };          // 0xA5: AND L
    opcode_table[0xA6] = [this]() { and_a_hl_mem(); };     // 0xA6: AND (HL)
    opcode_table[0xA7] = [this]() { and_a_a(); };          // 0xA7: AND A
    opcode_table[0xA8] = [this]() { xor_a_b(); };          // 0xA8: XOR B
    opcode_table[0xA9] = [this]() { xor_a_c(); };          // 0xA9: XOR C
    opcode_table[0xAA] = [this]() { xor_a_d(); };          // 0xAA: XOR D
    opcode_table[0xAB] = [this]() { xor_a_e(); };          // 0xAB: XOR E
    opcode_table[0xAC] = [this]() { xor_a_h(); };          // 0xAC: XOR H
    opcode_table[0xAD] = [this]() { xor_a_l(); };          // 0xAD: XOR L
    opcode_table[0xAE] = [this]() { xor_a_hl_mem(); };     // 0xAE: XOR (HL)
    opcode_table[0xAF] = [this]() { xor_a_a(); };          // 0xAF: XOR A                                                               

    opcode_table[0xB0] = [this]() { or_a_b(); };           // 0xB0: OR B
    opcode_table[0xB1] = [this]() { or_a_c(); };           // 0xB1: OR C
    opcode_table[0xB2] = [this]() { or_a_d(); };           // 0xB2: OR D
    opcode_table[0xB3] = [this]() { or_a_e(); };           // 0xB3: OR E
    opcode_table[0xB4] = [this]() { or_a_h(); };           // 0xB4: OR H
    opcode_table[0xB5] = [this]() { or_a_l(); };           // 0xB5: OR L
    opcode_table[0xB6] = [this]() { or_a_hl_mem(); };      // 0xB6: OR (HL)
    opcode_table[0xB7] = [this]() { or_a_a(); };           // 0xB7: OR A
    opcode_table[0xB8] = [this]() { cp_a_b(); };           // 0xB8: CP B
    opcode_table[0xB9] = [this]() { cp_a_c(); };           // 0xB9: CP C
    opcode_table[0xBA] = [this]() { cp_a_d(); };           // 0xBA: CP D
    opcode_table[0xBB] = [this]() { cp_a_e(); };           // 0xBB: CP E
    opcode_table[0xBC] = [this]() { cp_a_h(); };           // 0xBC: CP H
    opcode_table[0xBD] = [this]() { cp_a_l(); };           // 0xBD: CP L
    opcode_table[0xBE] = [this]() { cp_a_hl_mem(); };      // 0xBE: CP (HL)
    opcode_table[0xBF] = [this]() { cp_a_a(); };           // 0xBF: CP A


    opcode_table[0xC0] = [this]() { ret_nz(); };           // 0xC0: RET NZ
    opcode_table[0xC1] = [this]() { pop_bc(); };           // 0xC1: POP BC
    opcode_table[0xC2] = [this]() { jp_nz_nn(); };         // 0xC2: JP NZ, nn
    opcode_table[0xC3] = [this]() { jp_nn(); };            // 0xC3: JP nn
    opcode_table[0xC4] = [this]() { call_nz_nn(); };       // 0xC4: CALL NZ, nn
    opcode_table[0xC5] = [this]() { push_bc(); };          // 0xC5: PUSH BC
    opcode_table[0xC6] = [this]() { add_a_n(); };          // 0xC6: ADD A, n
    opcode_table[0xC7] = [this]() { rst_00h(); };          // 0xC7: RST 00H
    opcode_table[0xC8] = [this]() { ret_z(); };            // 0xC8: RET Z
    opcode_table[0xC9] = [this]() { ret(); };              // 0xC9: RET
    opcode_table[0xCA] = [this]() { jp_z_nn(); };          // 0xCA: JP Z, nn
    opcode_table[0xCB] = [this]() { cb_prefix(); };        // 0xCB: CB Prefix
    opcode_table[0xCC] = [this]() { call_z_nn(); };        // 0xCC: CALL Z, nn
    opcode_table[0xCD] = [this]() { call_nn(); };          // 0xCD: CALL nn
    opcode_table[0xCE] = [this]() { adc_a_n(); };          // 0xCE: ADC A, n
    opcode_table[0xCF] = [this]() { rst_08h(); };          // 0xCF: RST 08H

    opcode_table[0xD0] = [this]() { ret_nc(); };           // 0xD0: RET NC
    opcode_table[0xD1] = [this]() { pop_de(); };           // 0xD1: POP DE
    opcode_table[0xD2] = [this]() { jp_nc_nn(); };         // 0xD2: JP NC, nn
    opcode_table[0xD4] = [this]() { call_nc_nn(); };       // 0xD4: CALL NC, nn
    opcode_table[0xD5] = [this]() { push_de(); };          // 0xD5: PUSH DE
    opcode_table[0xD6] = [this]() { sub_a_n(); };          // 0xD6: SUB A, n
    opcode_table[0xD7] = [this]() { rst_10h(); };          // 0xD7: RST 10H
    opcode_table[0xD8] = [this]() { ret_c(); };             // 0xD8: RET C
    opcode_table[0xD9] = [this]() { reti(); };             // 0xD9: RETI
    opcode_table[0xDA] = [this]() { jp_c_nn(); };          // 0xDA: JP C, nn
    opcode_table[0xDB] = [this]() { in_a_n(); };           // 0xDB: IN A, (n)
    opcode_table[0xDC] = [this]() { call_c_nn(); };        // 0xDC: CALL C, nn
    opcode_table[0xDE] = [this]() { sbc_a_n(); };          // 0xDE: SBC A, n
    opcode_table[0xDF] = [this]() { rst_18h(); };          // 0xDF: RST 18H

    opcode_table[0xE0] = [this]() { ld_ff_a(); };        // 0xE0: LD (FF00+n), A
    opcode_table[0xE1] = [this]() { pop_hl(); };         // 0xE1: POP HL
    opcode_table[0xE2] = [this]() { ld_ff_c(); };        // 0xE2: LD (FF00+C), A
    opcode_table[0xE5] = [this]() { push_hl(); };        // 0xE5: PUSH HL
    opcode_table[0xE6] = [this]() { and_n(); };          // 0xE6: AND n
    opcode_table[0xE7] = [this]() { rst_20h(); };        // 0xE7: RST 20H
    opcode_table[0xE8] = [this]() { add_sp_n(); };       // 0xE8: ADD SP, n
    opcode_table[0xE9] = [this]() { jp_hl(); };          // 0xE9: JP (HL)
    opcode_table[0xEA] = [this]() { ld_nn_a(); };        // 0xEA: LD (nn), A
    opcode_table[0xEB] = [this]() { prefix_cb(); };      // 0xEB: CB Prefix
    opcode_table[0xEC] = [this]() { no_operation(); };   // 0xEC: NO OPERATION (unused in Game Boy)
    opcode_table[0xED] = [this]() { ed_prefix(); };      // 0xED: ED Prefix
    opcode_table[0xEE] = [this]() { xor_n(); };          // 0xEE: XOR n
    opcode_table[0xEF] = [this]() { rst_28h(); };        // 0xEF: RST 28H

    opcode_table[0xF0] = [this]() { ld_a_ff00_n(); };    // 0xF0: LD A, (FF00+n)
    opcode_table[0xF1] = [this]() { pop_af(); };         // 0xF1: POP AF
    opcode_table[0xF2] = [this]() { ld_a_ff00_c(); };    // 0xF2: LD A, (FF00+C)
    opcode_table[0xF3] = [this]() { di(); };             // 0xF3: DI
    opcode_table[0xF4] = [this]() { no_operation(); };   // 0xF4: NO OPERATION (unused in Game Boy)
    opcode_table[0xF5] = [this]() { push_af(); };        // 0xF5: PUSH AF
    opcode_table[0xF6] = [this]() { or_n(); };           // 0xF6: OR n
    opcode_table[0xF7] = [this]() { rst_30h(); };        // 0xF7: RST 30H
    opcode_table[0xF8] = [this]() { ld_hl_sp_n(); };     // 0xF8: LD HL, SP+n
    opcode_table[0xF9] = [this]() { ld_sp_hl(); };       // 0xF9: LD SP, HL
    opcode_table[0xFA] = [this]() { ld_a_nn(); };        // 0xFA: LD A, (nn)
    opcode_table[0xFB] = [this]() { ei(); };             // 0xFB: EI
    opcode_table[0xFC] = [this]() { no_operation(); };   // 0xFC: NO OPERATION (unused in Game Boy)
    opcode_table[0xFD] = [this]() { fd_prefix(); };      // 0xFD: FD Prefix
    opcode_table[0xFE] = [this]() { cp_n(); };           // 0xFE: CP n
    opcode_table[0xFF] = [this]() { rst_38h(); };        // 0xFF: RST 38H

}


void CPU::push_af(){
}
void CPU::or_n(){
}
void CPU::rst_30h(){
}
void CPU::ld_hl_sp_n(){
}
void CPU::ld_sp_hl(){
}
void CPU::ld_a_nn(){
}
void CPU::ei(){
}
void CPU::ret_c(){
}
void CPU::reti(){
}
void CPU::jp_c_nn(){
}
void CPU::in_a_n(){
}
void CPU::call_c_nn(){
}
void CPU::sbc_a_n(){
}
void CPU::rst_18h(){
}

void CPU::ld_ff_a(){
}
void CPU::pop_hl(){
}
void CPU::ld_ff_c(){
}
void CPU::push_hl(){
}
void CPU::and_n(){
}
void CPU::rst_20h(){
}
void CPU::add_sp_n(){
}
void CPU::jp_hl(){
}
void CPU::ld_nn_a(){
}
void CPU::prefix_cb(){
}
void CPU::no_operation(){
}

void CPU::adc_a_a() {
}

void CPU::jr_nc_n() {
}
void CPU::load_b_b() {
}
void CPU::load_b_c() {
}
void CPU::load_sp_nn() {
}
void CPU::load_b_d(){
}
void CPU::load_b_e() {
}
void CPU::jr_c_n() {
}
void CPU::load_b_h() {
}
void CPU::load_bc_nn() {
}
void CPU::load_b_n() {
}
void CPU::nop() {
}
void CPU::nn() {
}
void CPU::load_bc_a() {
}
void CPU::load_a_bc() {
}
void CPU::load_c_n() {
}
void CPU::stop() {
}
void CPU::load_de_nn() {
}

void CPU::inc_bc() {
}
void CPU::inc_b() {
}
void CPU::dec_b() {
}

void CPU::rlca() {
}
void CPU::load_nn_sp() {
}
void CPU::add_hl_bc() {
}

void CPU::dec_bc() {
}
void CPU::inc_c() {
}
void CPU::dec_c()   {
}
void CPU::rrca() {
}

void CPU::load_de_a() {
}
void CPU::inc_de() {
}
void CPU::inc_d(){
}
void CPU::dec_d(){
}
void CPU::load_d_n() {
}
void CPU::rla()  {
}
void CPU::jr_n() {
}
void CPU::add_hl_de() {
}
void CPU::load_a_de() {
}
void CPU::dec_de()  {
}
void CPU::inc_e()  {
}
void CPU::dec_e()  {
}
void CPU::load_e_n() {
}
void CPU::rra()     {
}
void CPU::jr_nz_n() {
}
void CPU::load_hl_nn()  {
}
void CPU::load_hl_plus_a() {
}
void CPU::inc_hl() {
}
void CPU::inc_h() {
}
void CPU::dec_h() {
}
void CPU::load_h_n() {
}
void CPU::daa() {
}
void CPU::jr_z_n() {
}
void CPU::add_hl_hl() {
}
void CPU::load_a_hl_plus() {
}
void CPU::dec_hl() {
}
void CPU::inc_l() {
}
void CPU::dec_l() {
}
void CPU::load_l_n() {
}
void CPU::cpl() {
}


void CPU::load_hl_minus_a() {
}

void CPU::inc_sp() {
}

void CPU::hl_mem() {
}

void CPU::load_hl_n() {
}

void CPU::scf() {
}


void CPU::add_hl_sp() {
}

void CPU::load_a_hl_minus() {
}

void CPU::dec_sp() {
}

void CPU::inc_a() {
}

void CPU::dec_a() {
}

void CPU::load_a_n() {
}

void CPU::ccf() {
}


void CPU::load_a_e() {
}

void CPU::load_a_h() {
}

void CPU::load_a_l() {
}

void CPU::load_a_hl_mem() {
}

void CPU::load_a_a() {
}

void CPU::sub_a_b() {
}

void CPU::sub_a_c() {
}

void CPU::sub_a_d() {
}

void CPU::sub_a_e() {
}

void CPU::sub_a_h() {
}

void CPU::sub_a_l() {
}


void CPU::sub_a_a() {
}

void CPU::sbc_a_b() {
}

void CPU::sbc_a_c() {
}

void CPU::sbc_a_d() {
}

void CPU::sbc_a_e() {
}

void CPU::sbc_a_h() {
}

void CPU::sbc_a_l() {
}


void CPU::sbc_a_a() {
}

void CPU::and_a_b() {
}

void CPU::and_a_c() {
}

void CPU::and_a_d() {
}

void CPU::and_a_e() {
}

void CPU::and_a_h() {
}

void CPU::and_a_l() {
}

void CPU::and_a_hl_mem() {
}

void CPU::and_a_a() {
}

void CPU::xor_a_b() {
}

void CPU::xor_a_c() {
}

void CPU::xor_a_d() {
}

void CPU::xor_a_e() {
}

void CPU::xor_a_h() {
}

void CPU::xor_a_l() {
}

void CPU::xor_a_hl_mem() {
}

void CPU::xor_a_a() {
}

void CPU::or_a_d() {
}
void CPU::or_a_e() {
}
void CPU::or_a_h() {
}
void CPU::or_a_l() {
}
void CPU::or_a_hl_mem() {
}
void CPU::or_a_a() {
}
void CPU::cp_a_b() {
}
void CPU::cp_a_c() {
}
void CPU::cp_a_d() {
}
void CPU::cp_a_e() {
}
void CPU::cp_a_h() {
}
void CPU::cp_a_l() {
}
void CPU::cp_a_hl_mem() {
}
void CPU::cp_a_a() {
}


void CPU::call_nz_nn(){
}
void CPU::push_bc(){
}
void CPU::add_a_n(){
}
void CPU::rst_00h(){
}
void CPU::ret_z(){
}
void CPU::ret(){
}
void CPU::jp_z_nn(){
}
void CPU::cb_prefix(){
}
void CPU::call_z_nn(){
}
void CPU::call_nn(){
}
void CPU::adc_a_n(){
}
void CPU::rst_08h(){
}
void CPU::ret_nc(){
}
void CPU::pop_de(){
}
void CPU::jp_nc_nn(){
}
void CPU::call_nc_nn() {
}
void CPU::push_de(){
}
void CPU::sub_a_n(){
}
void CPU::rst_10h(){
}

void CPU::emulate_cycle() {
    uint8_t opcode = romData[pc];
    pc++; 

    uint32_t cycleCount = opcode_cycles[opcode]; 
}


void CPU::step() {
    const uint32_t timeSlice = 1000; 
    const double cyclesPerMicrosecond = 4.194304; 
    uint32_t cyclesToRun = timeSlice * cyclesPerMicrosecond;

    while (cyclesToRun > 0) {
        emulate_cycle(); 
        // cyclesToRun -= cycles; 
    }
}

void CPU::emulate_cycles(uint32_t cyclesToRun) {
        uint8_t opcode ;
        while (cyclesToRun > 0) {
            // Fetch and decode opcode
            opcode = romData[pc];
            pc++;

            // Execute instruction and get cycle count
            uint32_t cycleCount = execute_opcode(opcode);

            // Update global cycle counter
            globalCycles += cycleCount;

            // Decrement cycles to run
            cyclesToRun -= cycleCount;

            // Handle other components (GPU, APU) based on elapsed cycles
            // updateComponents(cycleCount);
        }
}

uint32_t CPU::execute_opcode(uint8_t opcode) {
    uint32_t cycleCount = opcode_cycles[opcode]; 
    opcode_table[opcode]();

    return cycleCount;
}


std::vector<uint8_t> CPU::load_rom(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open ROM file." << std::endl;
        exit(1);
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (!file.read(reinterpret_cast<char*>(romData.data()), size)) {
        std::cerr << "Error: Could not read ROM file." << std::endl;
        exit(1);
    }

    file.close();
    return romData;
}

