#ifndef SRC_PPU_HPP_
#define SRC_PPU_HPP_

// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
#include <SDL2/SDL.h>
#include <cstdio>

class PPU {

private:
  uint32_t frambuffer[160 * 144];

public:
  void updatebuffer();
};
#endif // SRC_PPU_HPP_
