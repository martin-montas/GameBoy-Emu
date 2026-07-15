// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com

#include "serial.hpp"
#include <stdint.h>
#include <stdio.h>

void Serial::write(uint16_t addr, uint8_t value)
{
	if (addr == 0xFF01) {
		_sb = value;
	}

	if (addr == 0xFF02 && value == 0x81) {
		printf("%c", _sb);
		fflush(stdout);
	}
}

void Serial::tick(int cycle)
{
	if (_sc & TRANSFER_STATUS) {
		_acc += cycle;
		while (_acc >= 512) {
			_acc -= 512;
			shift_one_bit();
		}
	}
}

void Serial::shift_one_bit()
{
	uint8_t bit = (_shift_reg & 0x80) ? 1 : 0;
	printf("%d", bit);
	_shift_reg <<= 1;
}

uint8_t Serial::read(uint16_t addr)
{
	if (addr == 0xFF01)
		return 0xFF;
	else
		return 0xFF;
}
