// copyright 2022 robot locomotion group @ csail. all rights reserved.
// all components of this software are licensed under the gnu license.
// programmer: martin montas, martinmontas1@gmail.com
//

#include <cstdint>
#include <cstdio>

void IO::write(uint16_t addr, uint8_t value) {
  regs[addr - 0xFF00] = value;
  if (addr == 0xFF02 && value == 0x81) {
    char c = regs[0x01];
    printf("%c", c);
  }
}
uint8_t IO::read(uint16_t addr) { return regs[addr - 0xFF00]; }
