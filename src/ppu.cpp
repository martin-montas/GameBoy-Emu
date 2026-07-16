// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#include "ppu.hpp"
#include <cstdio>

/*
 * @brief Handles hblank-related things like
 * moving the another scanline and
 * entering vblank.
 */
void Ppu::hblank_handler()
{
	/* checks if scan line has ended */
	if (_f_flag == bg) {
		if (_dot_clock == 456) {
			_dot_clock = 0;
			LY += 1;
			/* enters vblank */
			if (LY == 144) {
				// _interrupt->request_interrupt(INTERRUPT_VBLANK);
			} else if (LY == 154) {
				/* enters new frame */
				enter_mode_2();
			}
		}
	}
}

void Ppu::enter_mode_2()
{
	return;
}

void Ppu::scan_oam(int row, uint8_t lcdc)
{
	if (!(LCDC & FLAG_OBJ_ENABLE)) {
		return;
	}
	int	offset;
	uint8_t LY   = _bus->read8(0xFF44);
	int	high = (LCDC & FLAG_OBJ_SIZE) ? 16 : 8;

	for (obj = 0; obj < 10; obj++) {
		objs[obj].X = 0xff;
	}

	if ((lcdc & FLAG_OBJ_ENABLE) == 0) {
		return;
	}
	size_t obj_size = 0;
	for (int sprite_index = 0; sprite_index < 40; sprite_index++) {
		const oam_addr = 0xFE00 + (sprite_index * 4);

		uint8_t sprite_y   = _bus->read8(oam_addr);
		uint8_t sprite_x   = _bus->read8(oam_addr + 1);
		uint8_t tile_index = _bus->read8(oam_addr + 2);
		uint8_t attr	   = _bus->read8(oam_addr + 3);

		uint8_t actualY = spriteY - 16;
		uint8_t actualX = spriteX - 8;
		if (LY >= actualY && LY < actualY + high) {
			objs[obj_size] =
			    OBJ{actualX, actualY, tile_index, attr};
			obj_size += 1;
			if (obj_size >= 9) {
				obj_size = 0;
				break;
			}
		}
	}
}

// TODO run this function after window/background layer
void Ppu::update_buffer_sprite(int sprite_mode);
{
	for (int i = objs.size() - 1; i >= 0; i++) {
		// struct OBJ {
		// 	uint8_t X;    /* X location */
		// 	uint8_t Y;    /* Y location */
		// 	uint8_t tile; /* Cached tile */
		// 	uint8_t attr; /* attribute */
		// };
		uint8_t sprite_row = LY - obj[i].Y;
		bool	flip_x	   = (objs[i].attr & 0x20) != 0;
		bool	flip_y	   = (objs[i].attr & 0x40) != 0;
		bool	belowbg	   = (objs[i].attr & 0x80) != 0;
		int	tile_row =
		    flip_y ? (sprite_mode - 1 - sprite_row) : sprite_row;
		uint16_t tileAddress;

		if (sprite_mode == 16) {
			tile_index &= 0xFE;
			if (tile_row >= 8) {
				tile_index += 1;
				tileAddress = 8000 + ((tile_index + 1) * 16);
				tile_row -= 8;
			}
		} else {
			tileAddress = 8000 + ((tile_index) * 16);
			tile_row    = tile_row;
		}

		uint16_t tile_addr = _bus->read8(0x8000 + (obj[i].tile * 16));

		uint8_t byte0 = _bus->read8(tile_addr + (tile_row * 2));
		uint8_t byte1 = _bus->read8(tile_addr + (tile_row * 2) + 1);

		for (int pix = 0; pix < 8; pix++) {
			uint8_t screen_x = objs[i].X + pix;

			int	bit_index = flip_x ? pixel_x : (7 - pixel_x);
			bool	msb	  = ((byte0 >> (7 - bitIndex)) & 1);
			bool	lsb	  = ((byte1 >> (7 - bitIndex)) & 1);
			uint8_t color	  = (msb << 1) | lsb;

			if (color == 0) {
				continue;
			}
			uint32_t color_val;
			if (color == 0) {
				color_val = WHITE;
			} else if (color == 1) {
				color_val = LIGHT_GRAY;
			} else if (color == 2) {
				color_val = DARK_GRAY;
			} else {
				color_val = BLACK;
			}
			if (belowbg) {
				if ()
			}

			frame_buff[LY * WIDTH + x] = color_val;
		}
	}
}
/*
 * @brief: Fetches pixels from tile data
 * and current tile map. and updates
 * the frame buffer array.
 *
 * @param[in]: address of selected tile map
 */
void Ppu::update_framebuff()
{
	bool	window	       = false;
	int	rendered_px    = 0;
	uint8_t _wy	       = _bus->read8(0xFF4A);
	uint8_t _wx	       = _bus->read8(0xFF4B);
	uint8_t LCDC	       = _bus->read8(0xFF40);
	bool	window_enabled = LCDC & FLAG_WIN_ENABLE;
	bool	window_active  = window_enabled && (LY >= _wy);

	for (int x = 0; x < 160; x++) {
		bool window_fires = window_active && (x >= _wx - 7);

		if (window_fires) {
			win_used     = true;
			uint8_t px_x = x - (_wx - 7);
			uint8_t px_y = wl_counter;

			int tile_x = px_x / 8;
			int tile_y = px_y / 8;

			int	 offset = tile_y * 32 + tile_x;
			uint8_t	 tile_index;
			uint16_t addr;

			if (!(_LCDC & FLAG_WIN_MAP)) {
				addr = 0x9800;
			} else {
				addr = 0x9C00;
			}
			uint8_t	 tile_number = _bus->read8(addr + offset);
			uint16_t tile_data_addr;

			if (_LCDC & FLAG_BG_AREA) {
				tile_data_addr = 0x8000 + (tile_number * 16);
			} else {
				int8_t signed_index = (int8_t)tile_number;
				tile_data_addr = 0x9000 + (signed_index * 16);
			}

			int	pixel_y = px_y % 8;
			int	pixel_x = px_x % 8;
			uint8_t byte0 =
			    _bus->read8(tile_data_addr + (pixel_x * 2));
			uint8_t byte1 =
			    _bus->read8(tile_data_addr + (pixel_y * 2) + 1);
			bool	msb   = ((byte0 >> (7 - px_y)) & 1);
			bool	lsb   = ((byte1 >> (7 - px_x)) & 1);
			uint8_t color = (msb << 1) | lsb;

			uint32_t color_val;
			if (color == 0) {
				color_val = WHITE;
			} else if (color == 1) {
				color_val = LIGHT_GRAY;
			} else if (color == 2) {
				color_val = DARK_GRAY;
			} else {
				color_val = BLACK;
			}
			frame_buff[LY * WIDTH + x] = color_val;
			wl_counter += 1;
		} else {
			uint8_t _scy	     = _bus->read8(0xFF42);
			uint8_t _scx	     = _bus->read8(0xFF43);
			uint8_t background_x = (x + _scx);
			uint8_t background_y = (LY + _scy);
			int	tile_x	     = background_x / 8;
			int	tile_y	     = background_y / 8;

			int	 offset = (tile_y * 32 + tile_x);
			uint16_t addr;

			if (!(_LCDC & FLAG_BG_MAP)) {
				addr = 0x9800;
			} else {
				addr = 0x9C00;
			}
			uint8_t	 tile_number = _bus->read8(addr + offset);
			uint16_t tile_data_addr;

			if (_LCDC & FLAG_BG_AREA) {
				tile_data_addr = 0x8000 + (tile_number * 16);
			} else {
				int8_t signed_index = (int8_t)tile_number;
				tile_data_addr = 0x9000 + (signed_index * 16);
			}
			int pixel_y = background_y % 8;
			int pixel_x = background_x % 8;

			uint8_t byte0 =
			    _bus->read8(tile_data_addr + (pixel_y * 2));
			uint8_t byte1 =
			    _bus->read8(tile_data_addr + (pixel_y * 2) + 1);

			bool msb = ((byte0 >> (7 - pixel_x)) & 1);
			bool lsb = ((byte1 >> (7 - pixel_x)) & 1);

			uint8_t	 color = (msb << 1) | lsb;
			uint32_t color_val;
			if (color == 0) {
				color_val = WHITE;
			} else if (color == 1) {
				color_val = LIGHT_GRAY;
			} else if (color == 2) {
				color_val = DARK_GRAY;
			} else {
				color_val = BLACK;
			}
			frame_buff[LY * WIDTH + x] = color_val;
		}
	}
}

void Ppu::fetch_sprites() {}

void Ppu::enter_mode_3()
{
	update_framebuff();
}

void Ppu::switch_mode(int mode)
{
	if (mode == 0)
		hblank_handler();
	else if (mode == 2)
		enter_mode_2();
	else if (mode == 3)
		enter_mode_3();
}

bool Ppu::frame_ready() const
{
	return can_render;
}

void Ppu::dot_cycle(int t_cycle)
{
	_LCDC = _bus->read8(LCDC_ADDR);
	if ((_LCDC & FLAG_LCD_ENABLE) == 0) {
		LY	   = 0;
		wl_counter = 0;
		_mode	   = 0;
		_dot_clock = 0;
		win_used   = false;
		return;
	}
	_dot_clock += t_cycle;

	if ((_LCDC & FLAG_WIN_ENABLE) && (_LCDC & FLAG_WIN_ENABLE))
		_f_flag = win;

	else if (!(_LCDC & FLAG_WIN_ENABLE) && (_LCDC & FLAG_BG_ENABLE))
		_f_flag = bg;
	else
		_f_flag = obj;

	switch (_mode) {
	case 0: /* HBlank */
		if (_dot_clock >= 204) {
			_dot_clock -= 204;
			LY += 1;
			uint8_t LYC = _bus->read8(0xFF45);
			if (LY == LYC) {
				_interrupt->request_interrupt(INTERRUPT_LCD);
			}
			if (LY == 144) {
				wl_counter = 0;
				_mode	   = 1;
				can_render = true;
				LY	   = 0;
			} else {
				_mode = 2;
				switch_mode(2);
			}
		}
		break;
	case 1: /* vblank */
		if (_dot_clock >= 456) {
			_dot_clock -= 456;
			LY += 1;
			if (LY == 154) {
				_mode = 2;
				LY    = 0;
				switch_mode(2);
			}
		}
		break;
	case 2: /* oam scan */
		if (_dot_clock >= 80) {
			_dot_clock -= 80;
			_mode = 3;
			switch_mode(3);
		}
		break;
	case 3: /* render */
		if (_dot_clock >= 172) {
			_dot_clock -= 172;
			_mode = 0;
			switch_mode(0);
		}
		break;
	}
}
