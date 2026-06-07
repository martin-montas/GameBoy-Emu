// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
#ifndef SRC_TEST_RUNNER_HPP_
#define SRC_TEST_RUNNER_HPP_

#include <nlohmann/json.hpp>
#include "cpu.hpp"
#include "sst-bus.hpp"
#include "instructions.hpp"

using j = nlohmann::json;
class TestRunner {
  private:
    Cpu*            cpu;
    SST*            mmu;
    InstructionSet* instruction;

  public:
    bool runFile();
    void run_cpu_test(const std::string file);
    void run_ppu_test(const std::string file);

    void verify_final_state(j final);
    void load_initial_state(j initial);
};

#endif // SRC_TEST_RUNNER_HPP_
