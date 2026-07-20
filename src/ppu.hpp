// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#ifndef SRC_PPU_HPP_
#define SRC_PPU_HPP_

#include <stdio.h>
#include <stdint.h>
#include <vector>
#include "bus.hpp"
#include "interface-interrupt.hpp"
#include "sdl-utils.hpp"

#define BGP_ADDR  0xFF47
#define LCDC_ADDR 0xFF40
#define WY_ADDR	  0xFF4A
#define WX_ADDR	  0xFF4B

#define TY 0
#define TX 1
#define TT 2
#define TF 3

#define WHITE	   0x8B956D
#define LIGHT_GRAY 0x4D784E
#define DARK_GRAY  0x2b543d
#define BLACK	   0x183020

struct OBJ {
	uint8_t X;	    /* X location */
	uint8_t Y;	    /* Y location */
	uint8_t tile_index; /* Cached tile */
	uint8_t attr;	    /* attribute */
};

/*
 * @brief: helper fot the lcdc flag.
 * Can get/set bits for the
 * lcd register.
 */
enum LCDFlag {
	FLAG_LCD_ENABLE = (1 << 7), /* checks if lcd should be on */
	FLAG_WIN_MAP	= (1 << 6), /* says which map should window use*/
	FLAG_WIN_ENABLE = (1 << 5), /* checks window memember off/on */
	FLAG_BG_AREA	= (1 << 4), /* checks bg area */
	FLAG_BG_MAP	= (1 << 3), /* says which map should bg use */
	FLAG_OBJ_SIZE	= (1 << 2), /* checks sprites (oam) size */
	FLAG_OBJ_ENABLE = (1 << 1), /* whether the sprites are enabled */
	FLAG_BGWIN_PRIORITY =
	    (1 << 0) /* checks whether background is enabled */
};

/*
 * @brief: use to know current type of
 * lcd members should be rendered at a
 * given moment.
 */
enum bgwin_priority {
	bg  = 0, /* background member */
	win = 1, /* window member */
	obj = 2	 /* sprite oam member */
};

class IInterrupt;
class Bus;
class SystemBus;
/*
 * @brief: deals with pixels rendering and lcd related
 * registers from  the game boy.
 */
class Ppu
{
	SystemBus*	    _bus;	/* pointer to memory object */
	IInterrupt*	    _interrupt; /* pointer to interrupt */
	size_t		    _dot_clock; /* updates the t cycles */
	size_t		    _mode;	/* updates to current mode */
	uint8_t		    _LCDC;	/* lcdc register */
	uint8_t		    _win_line;	/* used for window y */
	uint8_t		    LY;		/* LY register for scanlines bg */
	uint8_t		    wl_counter; /* WY register for scanlines win */
	bgwin_priority	    _f_flag;	/* updates ppu rendering component */
	bool		    win_used;	/* for window rendering */
	std::array<OBJ, 10> objs{};

      public:
	inline explicit Ppu(IInterrupt* interrupt)
	    : can_render(false), _mode(2), LY(0)
	{
		_interrupt = interrupt;

		/* updates frame buffer to black */
		for (int i = 0; i < WIDTH * HEIGHT; i++) {
			frame_buff[i] = 0xFF000000; // black
		}

		/* Initialize objects to invalid */
		for (int obj = 0; obj < 10; obj++) {
			objs[obj].X	     = 0xff;
			objs[obj].Y	     = 0xff;
			objs[obj].tile_index = 0;
			objs[obj].attr	     = 0;
		}
	}
	/*
	 * @brief: gets interrupt object for later use.
	 * @param[in]: bus object pointer.
	 * @param[in] interrupt object pointer.
	 */
	inline void attach(SystemBus* bus)
	{
		_bus = bus;
	}
	/*
	 * @brief: Used by memory subsystem
	 * to read the ly register from
	 * the ppu.
	 * @return: the ly register
	 */

	inline uint8_t read_ly()
	{
		return LY;
	}

	/*
	 * @brief: makes the memory
	 * management unit write
	 * to the ppu ly register
	 */
	inline void reset_ly(uint8_t value)
	{
		LY = 0x00;
	}

	/*
	 * @brief: frame buffer used on every vblank
	 * mode of the ppu. the size its 160x144
	 */
	uint32_t frame_buff[HEIGHT * WIDTH];
	bool	 frame_ready() const;
	void	 dot_cycle(int t_cycle);

	inline void clear_can_render()
	{
		can_render = false;
	}
	bool can_render;

	/*
	 * @brief: handles each each mode
	 * seperately since the ppu for the
	 * gameboy is based on modes.
	 * @param[in] current T cycle.
	 */
	void mode_handler(int t_cycle);
	void enter_mode_3();
	void enter_mode_2();
	void hblank_handler();

	void update_framebuff();

	void write_reg(uint8_t& data, uint16_t addr);
	void switch_mode(int mode);
	void render_frame();
	void fetch_sprites();

	void update_buffer_sprite(int obj_mode);
	void scan_oam();
};
#endif // SRC_PPU_HPP_
