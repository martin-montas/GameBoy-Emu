// copyright 2022 robot locomotion group @ csail. all rights reserved.
// all components of this software are licensed under the gnu license.
// programmer: martin montas, martinmontas1@gmail.com
//
#ifndef SRC_IO_HPP_
#define SRC_IO_HPP_

#include "MBC.hpp"
#include <cstdint>
#include <memory>

class IO {
private:
  uint8_t SB;
  uint8_t SC;
  std::unique_ptr<MBC> mbc;
  uint8_t reg[0x80] = {};

public:
  uint8_t read(uint16_t addr);
  void write(uint16_t addr, uint8_t value);
  // uint8_t &io_reg(uint16_t addr);
};

#endif // SRC_IO_HPP_
