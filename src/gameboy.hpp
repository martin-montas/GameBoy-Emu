// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#ifndef SRC_GAME_BOY_HPP_
#define SRC_GAME_BOY_HPP_

#include <stdint.h>
#include <string>

#include "cpu.hpp"
#include "sdl-utils.hpp"
#include "instructions.hpp"
#include "mmu.hpp"
#include "serial.hpp"
#include "timer.hpp"
#include "interrupt.hpp"
#include "ppu.hpp"

/*
 * @brief: starts the main GameBoy components and runs most of
 * subsystems. its the main class for the whole game.
 */
class GameBoy {
    Cpu*       _cpu;             /* pointer to cpu object */
    Serial*    _serial;          /* pointer to serial object */
    Timer*     _timer;           /* pointer to timer object */
    Mmu*       _mmu;             /* pointer to mmu object */
    Ppu*       _ppu;             /* pointer to ppu object */
    Interrupt* _interrupt;       /* pointer to interrupt object */
    SDL*       _sdl;             /* pointer to SDL2 object */
    bool       emulationRunning; /* bool to emulator running state */
    int        cycle_count = 0;  /* counter to CPU's T cycles */
    uint8_t    _opcode;          /* current instruction */

    void interrupt_handler();
    void unhalt();

  public:
    explicit GameBoy(const std::string file) {
        _interrupt = new Interrupt();
        _timer     = new Timer(_interrupt);
        _mmu       = new Mmu(file, _timer, _interrupt);
        _ppu       = new Ppu(_interrupt);
        _sdl       = new SDL();
        _cpu       = new Cpu(_ppu, _timer, _sdl, _mmu, _interrupt);
        _mmu->attach(_ppu);
        _ppu->attach(_mmu);
        _interrupt->attach(_cpu);

        emulationRunning = true;
        // _instructions->post_boot_state();
    }

    void run();
};
#endif // SRC_GAME_BOY_HPP_
