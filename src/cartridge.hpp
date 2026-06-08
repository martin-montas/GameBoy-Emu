// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com

#ifndef SRC_CARTRIDGE_HPP_
#define SRC_CARTRIDGE_HPP_

#include <stdint.h>

class Cartridge_bank {
  protected:
    uint8_t* _data;
    size_t   _size;
    uint32_t _bank;     /* current RAM bank  */
    uint32_t _mask;     /* current RAM bank  */
    uint32_t _ram_bank; /* current RAM bank  */
  public:
    explicit Cartridge_bank(uint8_t* data, size_t size)
        : _data(data), _size(size), _bank(0x4000), _ram_bank(0), _ram(NULL) {
        _mask = (uint32_t)size - 1;
    }
};

#endif // !SRC_CARTRIDGE_HPP_
