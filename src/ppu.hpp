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

#define WHITE      0xFFFFFFFF
#define LIGHT_GRAY 0xFFAAAAAA
#define DARK_GRAY  0xFF555555
#define BLACK      0xFF000000
#define SCALE      4

enum LCDFlag {
    FLAG_LCD_ENABLE = (1 << 7),
    FLAG_WIN_MAP    = (1 << 6),
    FLAG_WIN_ENBLE  = (1 << 5),
    FLAG_BG_AREA    = (1 << 4),
    FLAG_BG_MAP     = (1 << 3),
    FLAG_OBJ_SIZE   = (1 << 2),
    FLAG_OBJ_ENABLE = (1 << 1),
    FLAG_BG_ENABLE  = 1
};

class MMU;

class Ppu {
  private:
    MMU*    _mmu = nullptr;
    size_t  _dot_clock;
    size_t  _mode;
    uint8_t LCDC;
    uint8_t LY;
    uint8_t _scanline_counter;

    /*
     * @brief: this happens on mode 3 of the ppu.
     * where the value at 0xFF40 is read and
     */
    void mode_handler(int t_cycle);

    void enter_mode_3();
    void enter_mode_2();
    void hblank_handler();
    void update_framebuff(uint16_t addr);

    // do something like this
    // uint8_t read_reg(uint8_t& data, uint16_t addr);
    void write_reg(uint8_t& data, uint16_t addr);
    void switch_mode(int mode);
    void render_frame();
    void enter_mode_0();

  public:
    Ppu() : can_render(false), _mode(2), _scanline_counter(0), LY(0) {

        /* initilizes  frame buffer */
        for (int i = 0; i < WIDTH * HEIGHT; i++) {
            frame_buff[i] = 0xFF000000; // black
        }
    }
    ~Ppu();
    void attach(MMU* mmu) {
        _mmu = mmu;
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
