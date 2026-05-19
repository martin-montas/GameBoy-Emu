// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
//
#ifndef SRC_MMU_HPP_
#define SRC_MMU_HPP_

#include <cstdint>
#include <string>
#include <vector>

#define HRAM_SIZE 0x7F
#define IRAM_SIZE 8192
#define OAM_SIZE 0xA0
#define VRAM_SIZE 0x2000
#define WRAM_SIZE 0x2000
#define IO_REGISTERS_SIZE 0X80

class MMU {
public:
  MMU(std::string filename);
  std::vector<uint8_t> romData;
  uint8_t HRAM[HRAM_SIZE] = {};
  uint8_t IRAM[IRAM_SIZE] = {};

  uint8_t VRAM[VRAM_SIZE] = {};
  uint8_t WRAM[WRAM_SIZE] = {};
  uint8_t OAM[OAM_SIZE] = {};

  uint8_t IO_REGISTERS[IO_REGISTERS_SIZE] = {};
  uint8_t EXTERNAL_RAM[8192] = {};

  // auto InterruptEnabled;
  uint8_t read8(uint16_t address);
  uint16_t read16(uint16_t address);

  void write8(uint16_t address, uint8_t value);
  void write16(uint16_t address, uint16_t value);

  void load_rom(const std::string &filename);
};

#endif // SRC_MMU_HPP_
