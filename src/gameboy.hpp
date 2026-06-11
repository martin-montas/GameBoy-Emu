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
#include "ppu.hpp"

class GameBoy {
  private:
    bool            emulationRunning;
    uint8_t         _opcode;
    int             cycle_count = 0;
    Cpu*            _cpu;
    Serial*         _serial;
    Timer*          _timer;
    MMU*            _mmu;
    Ppu*            _ppu;
    InstructionSet* _instructions;
    SDL*            _sdl;

  public:
    GameBoy(const std::string file) {
        _cpu          = new Cpu();
        _mmu          = new MMU(file);
        _ppu          = new Ppu();
        _sdl          = new SDL();
        _instructions = new InstructionSet(_mmu, _cpu);
        _mmu->attach(_ppu);
        _ppu->attach(_mmu);

        emulationRunning = true;
        // _instructions->post_boot_state();
    }

    uint32_t calculateCyclesForFrame();
    void     run();
    void     step();
};
#endif // SRC_GAME_BOY_HPP_
