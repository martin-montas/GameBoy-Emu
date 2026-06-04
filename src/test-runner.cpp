// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
#include <cstdint>
#include <fstream>
// #include <iostream>
#include <nlohmann/json.hpp>
#include "test-runner.hpp"
#include "sst-bus.hpp"
#include "cpu.hpp"
#include "instructions.hpp"

void TestRunner::run_test(const std::string json_file) {
    std::ifstream  jsonFileStream(json_file);
    nlohmann::json jsonData = nlohmann::json::parse(jsonFileStream);
    cpu                     = new Cpu();
    mmu                     = new SST();
    instruction             = new InstructionSet(mmu, cpu);

    j initial = jsonData["initial"].get<uint8_t>();
    load_initial_state(initial);
    instruction->step();
    // verify_final_state(test["final"]);
}
void TestRunner::load_initial_state(j initial) {}

void TestRunner::verify_final_state(j final) {}
