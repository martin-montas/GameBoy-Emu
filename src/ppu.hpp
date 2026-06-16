// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#ifndef SRC_PPU_HPP_
#define SRC_PPU_HPP_

#include <stdio.h>
#include "mmu.hpp"
#include "sdl-utils.hpp"

#define BGP_ADDR  0xFF47
#define LCDC_ADDR 0xFF40
#define WY_ADDR   0xFF4A
#define WX_ADDR   0xFF4B

#define WHITE      0x9A9E3F
#define LIGHT_GRAY 0x496B22
#define DARK_GRAY  0x0E450B
#define BLACK      0x1B2A09
#define SCALE      4

/*
 * @brief: helper fot the lcdc flag.
 * Can get/set bits for the
 * lcd register.
 */
enum LCDFlag {
    FLAG_LCD_ENABLE = (1 << 7), /* checks if lcd should be on */
    FLAG_WIN_MAP    = (1 << 6), /* says which map should window use*/
    FLAG_WIN_ENABLE = (1 << 5), /* checks window memember off/on */
    FLAG_BG_AREA    = (1 << 4),
    FLAG_BG_MAP     = (1 << 3), /* says which map should bg use */
    FLAG_OBJ_SIZE   = (1 << 2), /* checks sprites (oam) size */
    FLAG_OBJ_ENABLE = (1 << 1), /* whether the sprites are enabled */
    FLAG_BG_ENABLE  = 1         /* checks whether background is enabled */
};

/*
 * @brief: use to know current type of
 * lcd members should be rendered at a
 * given moment.
 */
enum bgwin_priority {
    bg  = 0, /* background member */
    win = 1, /* window member */
    obj = 2  /* sprite oam member */
};

class IInterrupt;
class Mmu;
class SystemBus;
/*
 * @brief: deals with pixels rendering and lcd related
 * registers from  the game boy.
 */
class Ppu {
    SystemBus*     _mmu;       /* pointer to memory object */
    IInterrupt*    _interrupt; /* pointer to interrupt */
    size_t         _dot_clock; /* updates the t cycles */
    size_t         _mode;      /* updates to current mode */
    uint8_t        _LCDC;      /* lcdc register */
    uint8_t        LY;         /* LY register for scanlines */
    bgwin_priority _f_flag;    /* updates ppu rendering component */
    bool           y_cond;

  public:
    inline explicit Ppu(IInterrupt* interrupt) : can_render(false), _mode(2), LY(0) {
        _interrupt = interrupt;

        /* updates frame buffer to black */
        for (int i = 0; i < WIDTH * HEIGHT; i++) {
            frame_buff[i] = 0xFF000000; // black
        }
    }
    /*
     * @brief: gets interrupt object for later use.
     * @param[in]: mmu object pointer.
     * @param[in] interrupt object pointer.
     */
    inline void attach(SystemBus* mmu) {
        _mmu   = mmu;
        y_cond = false;
    }
    /*
     * @brief: Used by memory subsystem
     * to read the ly register from
     * the ppu.
     * @return: the ly register
     */

    inline uint8_t read_ly() {
        return LY;
    }

    /*
     * @brief: makes the memory
     * management unit write
     * to the ppu ly register
     */
    inline void write_ly(uint8_t value) {
        LY = 0;
    }

    /*
     * @brief: frame buffer used on every vblank
     * mode of the ppu. the size its 160x144
     */
    uint32_t frame_buff[HEIGHT * WIDTH];
    bool     frame_ready() const;
    void     dot_cycle(int t_cycle);

    inline void clear_can_render() {
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
    void bg_update_framebuff(uint16_t addr);
    void win_update_framebuff(uint16_t addr);

    void write_reg(uint8_t& data, uint16_t addr);
    void switch_mode(int mode);
    void render_frame();
};
#endif // SRC_PPU_HPP_
