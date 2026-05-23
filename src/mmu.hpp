// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
//
#ifndef SRC_MMU_HPP_
#define SRC_MMU_HPP_

#include "IO.hpp"
#include "MBC.hpp"
#include "timer.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#define HRAM_SIZE 0x7F
#define IRAM_SIZE 8192
#define OAM_SIZE 0xA0
#define VRAM_SIZE 0x2000
#define WRAM_SIZE 0x2000
#define IO_SIZE 0x80

class MMU {
public:
  std::vector<uint8_t> romData;
  MMU(std::string filename, IO *io, Timer *timer);
  uint8_t read8(uint16_t address);
  uint16_t read16(uint16_t address);

  void write8(uint16_t address, uint8_t value);
  void write16(uint16_t address, uint16_t value);
  void load_rom(const std::string &filename);

private:
  std::unique_ptr<MBC> mbc;
  IO *io;
  Timer *timer;
  void check_rom_type();
  uint8_t HRAM[HRAM_SIZE] = {};
  uint8_t IRAM[IRAM_SIZE] = {};
  uint8_t VRAM[VRAM_SIZE] = {};
  uint8_t WRAM[WRAM_SIZE] = {};
  uint8_t OAM[OAM_SIZE] = {};
  // uint8_t IO[IO_SIZE] = {};
  uint8_t EXTERNAL_RAM[8192] = {};

  // auto InterruptEnabled;
};

#endif // SRC_MMU_HPP_
