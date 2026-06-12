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

class GameBoy {
  private:
    bool            emulationRunning;
    int             cycle_count = 0;
    uint8_t         _opcode;
    Cpu*            _cpu;
    Serial*         _serial;
    Timer*          _timer;
    MMU*            _mmu;
    Ppu*            _ppu;
    InstructionSet* _instructions;
    Interrupt*      _interrupt;
    SDL*            _sdl;

    void interrupt_handler();
    void unhalt();

  public:
    GameBoy(const std::string file) {
        // pass Interrupt here:
        // TODO
        _interrupt = new Interrupt();
        _cpu       = new Cpu();
        _mmu       = new MMU(file, _interrupt);
        _ppu       = new Ppu();
        _sdl       = new SDL();

        _instructions = new InstructionSet(_mmu, _cpu);

        _mmu->attach(_ppu);
        _ppu->attach(_mmu, _interrupt);

        emulationRunning = true;
        // _instructions->post_boot_state();
    }

    void        run();
    inline void step();
};
#endif // SRC_GAME_BOY_HPP_
