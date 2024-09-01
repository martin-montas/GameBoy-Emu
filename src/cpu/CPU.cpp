#include <cstdint>
#include <iostream>
#include <fstream>

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
void CPU::load_hl_plus_a(){
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

void CPU::step() {
    // Emulate cycles for a fixed time slice
    const uint32_t timeSlice = 1000; // Time slice in microseconds
    const double cyclesPerMicrosecond = 4.194304; // 4.194304 MHz
    uint32_t cyclesToRun = timeSlice * cyclesPerMicrosecond;

    while (cyclesToRun > 0) {
        // emulateCycle(); // Execute one instruction
        // cyclesToRun -= cycles; // Decrement remaining cycles
    }
}

void CPU::read_rom(const std::vector<uint8_t> &romData) {
    uint8_t opcode;
    while (pc < romData.size()) {
        opcode = romData[pc];
        pc++; 

        opcode_table[opcode]();
    }
}
