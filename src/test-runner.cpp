// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
#include <cstdint>
#include <fstream>
#include <cassert>
#include <iostream>
#include <nlohmann/json.hpp>
#include "test-runner.hpp"
#include "sst-bus.hpp"
#include "cpu.hpp"
#include "instructions.hpp"

void TestRunner::run_test(const std::string json_file) {
    std::ifstream jsonFileStream(json_file);
    j             jsonData = j::parse(jsonFileStream);
    cpu                    = new Cpu();
    mmu                    = new SST();
    instruction            = new InstructionSet(mmu, cpu);

    const auto& test    = jsonData[0];
    const auto& initial = test["initial"];
    const auto& final   = test["final"];

    load_initial_state(initial);
    instruction->step();
    verify_final_state(final);
}
void TestRunner::load_initial_state(j initial) {
    cpu->PC   = initial["pc"];
    cpu->SP   = initial["sp"];
    cpu->A    = initial["a"];
    cpu->B    = initial["b"];
    cpu->C    = initial["c"];
    cpu->D    = initial["d"];
    cpu->E    = initial["e"];
    cpu->F    = initial["f"];
    cpu->H    = initial["h"];
    cpu->L    = initial["l"];
    cpu->_ime = initial["ime"];
    // mmu->_ie  = initial["ie"];

    j ram = initial["ram"];
    for (auto r : ram) {
        mmu->write8(r[0], r[1]);
    }
}
void TestRunner::verify_final_state(j final) {

    printf("-- mine:%04X  => final:%04X  --\n", cpu->PC, final["pc"].get<uint16_t>());
    if (cpu->PC != final["pc"].get<uint16_t>()) {

        printf("PC mismatch: expected %04X got %04X\n", final["pc"].get<uint16_t>(), cpu->PC);
        exit(1);
    }

    printf("-- mine:%04X  => final:%04X  --\n", cpu->SP, final["sp"].get<uint16_t>());
    if (cpu->SP != final["sp"].get<uint16_t>()) {
        printf("SP mismatch: expected %04X got %04X\n", final["sp"].get<uint16_t>(), cpu->SP);
        exit(1);
    }

    printf("-- mine:%04X  => final:%04X  --\n", cpu->A, final["a"].get<uint8_t>());
    if (cpu->A != final["a"].get<uint8_t>()) {
        printf("A mismatch: expected %02X got %02X\n", final["a"].get<uint8_t>(), cpu->A);
        exit(1);
    }

    printf("-- mine:%04X  => final:%04X  --\n", cpu->B, final["b"].get<uint8_t>());
    if (cpu->B != final["b"].get<uint8_t>()) {
        printf("B mismatch: expected %02X got %02X\n", final["b"].get<uint8_t>(), cpu->B);
        exit(1);
    }

    printf("-- mine:%04X  => final:%04X  --\n", cpu->C, final["c"].get<uint8_t>());
    if (cpu->C != final["c"].get<uint8_t>()) {
        printf("C mismatch: expected %02X got %02X\n", final["c"].get<uint8_t>(), cpu->C);
        exit(1);
    }

    printf("-- mine:%04X  => final:%04X  --\n", cpu->D, final["d"].get<uint8_t>());
    if (cpu->D != final["d"].get<uint8_t>()) {
        printf("D mismatch: expected %02X got %02X\n", final["d"].get<uint8_t>(), cpu->D);
        exit(1);
    }

    printf("-- mine:%04X  => final:%04X  --\n", cpu->E, final["e"].get<uint8_t>());
    if (cpu->E != final["e"].get<uint8_t>()) {
        printf("E mismatch: expected %02X got %02X\n", final["e"].get<uint8_t>(), cpu->E);
        exit(1);
    }

    printf("-- mine:%04X  => final:%04X  --\n", cpu->F, final["f"].get<uint8_t>());
    if (cpu->F != final["f"].get<uint8_t>()) {
        printf("F mismatch: expected %02X got %02X\n", final["f"].get<uint8_t>(), cpu->F);
        exit(1);
    }

    printf("-- mine:%04X  => final:%04X  --\n", cpu->H, final["h"].get<uint8_t>());
    if (cpu->H != final["h"].get<uint8_t>()) {
        printf("H mismatch: expected %02X got %02X\n", final["h"].get<uint8_t>(), cpu->H);
        exit(1);
    }

    printf("-- mine:%04X  => final:%04X  --\n", cpu->L, final["l"].get<uint8_t>());
    if (cpu->L != final["l"].get<uint8_t>()) {
        printf("L mismatch: expected %02X got %02X\n", final["l"].get<uint8_t>(), cpu->L);
        exit(1);
    }

    printf("-- mine:%04X  => final:%04X  --\n", cpu->_ime, final["ime"].get<uint8_t>());
    if (cpu->_ime != final["ime"].get<uint8_t>()) {
        printf("IME mismatch: expected %d got %d\n", final["ime"].get<uint8_t>(), cpu->_ime);
        exit(1);
    }

    for (const auto& r : final["ram"]) {
        uint16_t addr     = r[0].get<uint16_t>();
        uint8_t  expected = r[1].get<uint8_t>();
        uint8_t  actual   = mmu->read8(addr);

        printf("-- actual:%02X => expected:%02X --\n", actual, expected);
        if (actual != expected) {
            printf("RAM mismatch at %04X: expected %02X got %02X\n", addr, expected, actual);
            exit(1);
        }
    }

    printf("-- passed! --\n");
}
