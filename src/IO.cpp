#include<cstdint>
#include "IO.hpp"

void IO::write(uint16_t addr, uint8_t value) {
  switch (addr) {
  case 0xFF00 {
    break;
  } case 0xFF01 {
    this->SC = value;
    break;
  } case 0xFF02 {
    if (value == 0x81 {
	printf("-- %X --\n",value);
      }
    break;
  } case 0xFF04 {
  break;
} case 0xFF05 {
  break;
} case 0xFF06 {
  break;
} case 0xFF07 {
  break;
} case 0xFF0F {
  break;
} case 0xFF10 {
  break;
} case 0xFF11 {
  break;
} case 0xFF12 {
  break;
} case 0xFF13 {
  break;
} case 0xFF14 {
  break;
} case 0xFF16 {
  break;
} case 0xFF17 {
  break;
} case 0xFF18 {
  break;
} case 0xFF19 {
  break;
} case 0xFF1A {
  break;
} case 0xFF1B {
  break;
} case 0xFF1C {
  break;
} case 0xFF1D {
  break;
} case 0xFF1E {
  break;
} case 0xFF20 {
  break;
} case 0xFF21 {
  break;
} case 0xFF22 {
  break;
} case 0xFF23 {
  break;
} case 0xFF24 {
  break;
} case 0xFF25 {
  break;
} case 0xFF26 {
  break;
} case 0xFF30 {
  break;
} case FF3F {
  break;
} case 0xFF40 {
  break;
} case 0xFF41 {
  break;
} case 0xFF42 {
  break;
} case 0xFF43 {
  break;
} case 0xFF44 {
  break;
} case 0xFF45 {
  break;
} case 0xFF46 {
  break;
} case 0xFF47 {
  break;
} case 0xFF48 {
  break;
} case 0xFF49 {
  break;
} case 0xFF4A {
  break;
} case 0xFF4B {
  break;
} case 0xFF4C {
  break;
} case 0xFF4D {
  break;
} case 0xFF4F {
  break;
} case 0xFF50 {
  break;
} case 0xFF51 {
  break;
} case 0xFF52 {
  break;
} case 0xFF53 {
  break;
} case 0xFF54 {
  break;
} case 0xFF55 {
  break;
} case 0xFF56 {
  break;
} case 0xFF68 {
  break;
} case 0xFF69 {
  break;
} case 0xFF6A {
  break;
} case 0xFF6B {
  break;
} case 0xFF6C {
  break;
} case 0xFF70 {
  break;
} case 0xFF76 {
  break;
} case 0xFF77 {
  break;
} case 0xFFFF {
  break;
} case default: {
  break;
  }
  }
}
