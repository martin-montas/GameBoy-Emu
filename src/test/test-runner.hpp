// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#ifndef SRC_TEST_RUNNER_HPP_
#define SRC_TEST_RUNNER_HPP_

#include <nlohmann/json.hpp>
#include "../cpu.hpp"
#include "../mmu.hpp"
#include "../sdl-utils.hpp"
#include "../timer.hpp"
#include "../sst-bus.hpp"
#include "../ppu.hpp"
#include "../interface-interrupt.hpp"

class SystemBus;
class Mmu;
using j = nlohmann::json;
class TestRunner {
    Ppu*        ppu;
    Cpu*        cpu;
    Timer*      timer;
    SST*        mmu;
    SDL*        sdl;
    IInterrupt* interrupt;

  public:
    bool runFile();
    void run_cpu_test(const std::string file);
    void run_ppu_test(const std::string file);

    void verify_final_state(j final);
    void load_initial_state(j initial);
};

#endif // SRC_TEST_RUNNER_HPP_
