// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
#include <cstdint>
#include <fstream>
#include <iostream>
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

    const auto& test    = jsonData[0];
    const auto& initial = test["initial"];

    load_initial_state(initial);
    // const auto& init instruction->step();
    // verify_final_state(test["final"]);
}
void TestRunner::load_initial_state(j initial) {
    cpu->PC = initial["pc"];
    std::cout << cpu->PC << std ::endl;

    cpu->SP = initial["sp"];
    cpu->A  = initial["a"];
    cpu->B  = initial["b"];
    cpu->C  = initial["c"];
    cpu->D  = initial["d"];
    cpu->E  = initial["e"];
    cpu->F  = initial["f"];
    cpu->H  = initial["h"];
    cpu->L  = initial["l"];
}

void TestRunner::verify_final_state(j final) {}
