// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#ifndef SRC_PPU_HPP_
#define SRC_PPU_HPP_

#include <stdio.h>
#include "mmu.hpp"
#include "sdl-utils.hpp"

#define BGP_ADDR  0xFF47 // BGP palette (0xFF47)
#define LCDC_ADDR 0xFF40
#define WY_ADDR   0xFF4A
#define WX_ADDR   0xFF4B

#define WHITE      0x9A9E3F
#define LIGHT_GRAY 0x496B22
#define DARK_GRAY  0x0E450B
#define BLACK      0x1B2A09
#define SCALE      4

enum LCDFlag {
    FLAG_LCD_ENABLE = (1 << 7),
    FLAG_WIN_MAP    = (1 << 6),
    FLAG_WIN_ENABLE = (1 << 5),
    FLAG_BG_AREA    = (1 << 4),
    FLAG_BG_MAP     = (1 << 3),
    FLAG_OBJ_SIZE   = (1 << 2),
    FLAG_OBJ_ENABLE = (1 << 1),
    FLAG_BG_ENABLE  = 1
};

enum bgwin_priority {
    bg  = 0,
    win = 1,
    obj = 2

};

class MMU;

class Ppu {
  private:
    MMU*           _mmu = nullptr;
    size_t         _dot_clock;
    size_t         _mode;
    uint8_t        LCDC;
    uint8_t        LY;
    bgwin_priority _f_flag;
    bool           y_cond;

    /*
     * @brief: this happens on mode 3 of the ppu.
     * where the value at 0xFF40 is read and
     */
    void mode_handler(int t_cycle);

    void enter_mode_3();
    void enter_mode_2();
    void hblank_handler();
    void bg_update_framebuff(uint16_t addr);
    void win_update_framebuff(uint16_t addr);

    // do something like this
    // uint8_t read_reg(uint8_t& data, uint16_t addr);
    void write_reg(uint8_t& data, uint16_t addr);
    void switch_mode(int mode);
    void render_frame();

  public:
    Ppu() : can_render(false), _mode(2), LY(0) {

        /* initilizes  frame buffer */
        for (int i = 0; i < WIDTH * HEIGHT; i++) {
            frame_buff[i] = 0xFF000000; // black
        }
    }
    ~Ppu();
    void attach(MMU* mmu) {
        _mmu   = mmu;
        y_cond = false;
    }

    uint32_t frame_buff[HEIGHT * WIDTH];
    bool     frame_ready() const;
    void     dot_cycle(int t_cycle);
    void     clear_can_render() {
        can_render = false;
    }
    bool    can_render;
    uint8_t read_ly();
};
#endif // SRC_PPU_HPP_
