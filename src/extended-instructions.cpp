// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com

#include <cstdint>
#include <sys/types.h>

#include <stdint.h>

#include "cpu.hpp"
#include "instructions.hpp"

void InstructionSet::rl_extended(uint8_t &reg) {
  // DONE
  bool bit7 = reg & 0x80;
  reg = (reg << 1) | (cpu->F & FLAG_CARRY ? 1 : 0);

  if (bit7) {
    cpu->F |= FLAG_CARRY;
  } else {
    cpu->F &= ~FLAG_CARRY;
  }
}
void InstructionSet::rlc_extended(uint8_t &reg) {
  // DONE
  uint8_t bit7 = (reg & 0x80) >> 7;
  reg = (reg << 1);
  reg |= bit7;
  cpu->set_flag(FLAG_CARRY, bit7);
  cpu->set_flag(FLAG_ZERO, reg == 0);
  cpu->clear_flag(FLAG_HALF_CARRY);
  cpu->clear_flag(FLAG_SUBTRACT);
}

void InstructionSet::rcc_extended(uint8_t *reg) {
  // DONE
  bool bit0 = *reg & 1;
  cpu->F &= ~FLAG_CARRY;
  cpu->F |= (bit0 << 4);
  *reg >>= 1;

  if (bit0) {
    *reg |= 0x08;
  }
}

void InstructionSet::rrc_extended(uint8_t &reg) {
  // DONE: should be checked
  uint8_t bit0 = reg & 0x01;
  if (bit0) {
    cpu->F |= FLAG_CARRY;
  } else {
    cpu->F &= ~FLAG_CARRY;
  }
  reg >>= 1;
  if (cpu->F & FLAG_CARRY) {
    reg |= 0x80;
  }
  cpu->set_flag(FLAG_ZERO, reg == 0);
  cpu->clear_flag(FLAG_SUBTRACT);
  cpu->clear_flag(FLAG_HALF_CARRY);
}

void InstructionSet::sla_extended(uint8_t &reg) {
  // done
  bool b7 = reg & 0x80;
  reg <<= 1;
  cpu->set_flag(FLAG_ZERO, reg == 0);
  cpu->clear_flag(FLAG_SUBTRACT);
  cpu->clear_flag(FLAG_HALF_CARRY);
  cpu->set_flag(FLAG_CARRY, b7);
}

void InstructionSet::sra_extended(uint8_t &reg) {
  // DONE
  bool b0 = reg & 0x01;

  bool b7 = reg & 0x80;

  reg >>= 1;
  if (b7) {
    reg |= 0x80;
  }
  cpu->set_flag(FLAG_ZERO, reg == 0);
  cpu->set_flag(FLAG_CARRY, b0);
  cpu->clear_flag(FLAG_SUBTRACT);
  cpu->clear_flag(FLAG_HALF_CARRY);
}

void InstructionSet::rr_extended(uint8_t &reg) {
  // DONE
  bool c = cpu->F & FLAG_CARRY;
  bool b0 = reg & 0x01;
  reg >>= 1;
  if (c) {
    reg |= 0x80;
  } else {
    reg &= ~0x80;
  }
  cpu->set_flag(FLAG_ZERO, reg == 0);
  cpu->clear_flag(FLAG_SUBTRACT);
  cpu->clear_flag(FLAG_HALF_CARRY);
  cpu->set_flag(FLAG_CARRY, b0);
}

void InstructionSet::swap_extended(uint8_t &reg) {
  // DONE
  uint8_t ln = (reg & 0xF);
  uint8_t hn = (reg >> 4) & 0xF;
  reg = (ln << 4) | hn;
}

void InstructionSet::srl_extended(uint8_t &reg) {
  // DONE
  bool b0 = reg & 0x01;
  reg >>= 1;
  if (b0) {
    cpu->F |= 0x01;
  } else {
    cpu->F &= ~0x01;
  }
  cpu->set_flag(FLAG_ZERO, reg == 0);
  cpu->clear_flag(FLAG_SUBTRACT);
  cpu->clear_flag(FLAG_HALF_CARRY);
}
