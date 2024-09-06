
#include "registers.hpp"
#include "instructions.hpp"

InstructionSet::InstructionSet(Registers *registers, MMU *mmu){
    this->registers = registers;
    this->mmu = mmu;

    // todo(martin-montas) create this later
    // this->interrupts = interrupts;
}
void execute(uint8_t opcode);

void ret(bool condition) {
}
void xor_(uint8_t value) {
}
void inc(uint8_t *value) {
}
void dec(uint8_t *value) {
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

