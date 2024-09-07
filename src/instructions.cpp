#include "registers.hpp"
#include <cstdint>
#include "instructions.hpp"

void CPU::load_bc_nn() {
    pc += 1;
    uint8_t tmp_1 = romData[pc];
    pc += 1;
    uint8_t tmp_2 = romData[pc];
    registers->BC = (tmp_2 << 8) | tmp_1;
}

void CPU::load_b_n() {
    uint8_t tmp = romData[pc++];
    registers->B = tmp;
}


void CPU::load_bc_a() {
    uint16_t tmp = registers->BC;

    // video ram
    if (tmp >= 0x8000 && tmp <= 0x9FFF) {
        mmu->VRAM[tmp - 0x8000] = registers->A;
    }

    // external ram
    else if (tmp >= 0xA000 && tmp <= 0xBFFF) {
        mmu->EXTERNAL_RAM[tmp - 0xA000] = registers->A;
    }

    // working ram
    else if (tmp >= 0xC000 && tmp <= 0xDFFF) {
        mmu->WRAM[tmp - 0xC000] = registers->A;
    }

    // Echo ram
    else if (tmp >= 0xE000 && tmp <= 0xFDFF) {
        mmu->WRAM[tmp - 0xE000] = registers->A;
    }

    // oam memory
    else if (tmp >= 0xFE00 && tmp <= 0xFE9F) {
        mmu->OAM[tmp - 0xFE00] = registers->A;
    }

    // I/O registers
    else if (tmp >= 0xFF00 && tmp <= 0xFF7F) {
        mmu->IORegisters[tmp - 0xFF00] = registers->A;
    }

    // HRAM memory
    else if (tmp >= 0xFF80 && tmp <= 0xFFFE) {
        mmu->HRAM[tmp - 0xFF80] = registers->A;
    }

    // InterruptEnable registers
    else if (tmp == 0xFFFF) {
        mmu->InterruptEnabled = registers->A;
    }
}



InstructionSet::InstructionSet(Registers *registers, MMU *mmu){
    this->registers = registers;
    this->mmu = mmu;

    // todo(martin-montas) create this later
    // this->interrupts = interrupts;
}
void execute(uint8_t opcode) {
}
void ret(bool condition) {
}
void xor_(uint8_t value) {
}

void inc(uint16_t *value) {
    value++;
}

void inc(uint8_t *value) {
    value++;
}
void dec(uint8_t *value) {
    value--;
}

void dec(uint16_t *value) {
    value--;
}
void add(uint8_t *destination, uint8_t value) {
}
void add(uint16_t *destination, uint16_t value) {
}
void add(uint16_t *destination, int8_t value) {
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

// Extended instructions
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
}

