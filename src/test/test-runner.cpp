// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#include <cstdint>
#include <fstream>
#include <cassert>
#include <iostream>
#include <nlohmann/json.hpp>
#include "test-runner.hpp"
#include "../system-bus.hpp"
#include "../cpu.hpp"
#include "../timer.hpp"
#include "../sdl-utils.hpp"
#include "../interrupt.hpp"
#include "../ppu.hpp"

void TestRunner::run_cpu_test(const std::string file) {
    std::ifstream jsonFileStream(file);
    j             jsonData = j::parse(jsonFileStream);
    if (!jsonFileStream.is_open()) {
        printf("Failed to open file: %s\n", file.c_str());
        exit(1);
    }
    sdl       = new SDL();
    interrupt = new Interrupt();
    ppu       = new Ppu(interrupt);
    timer     = new Timer(interrupt);
    mmu       = new SST();
    cpu       = new Cpu(ppu, timer, sdl, mmu, interrupt);
    for (const auto& test : jsonData) {
        delete cpu;
        delete mmu;

        sdl       = new SDL();
        interrupt = new Interrupt();
        ppu       = new Ppu(interrupt);
        timer     = new Timer(interrupt);
        mmu       = new SST();
        cpu       = new Cpu(ppu, timer, sdl, mmu, interrupt);

        std::string name = test["name"].get<std::string>();
        printf("Running: %s\n", name.c_str());
        const auto& initial = test["initial"];
        load_initial_state(initial);
        cpu->step();
        const auto& final = test["final"];
        verify_final_state(final);
    }

    printf("=================================================\n");
    printf("------------------- passed! ---------------------\n");
    printf("=================================================\n");
}

void TestRunner::run_ppu_test(const std::string file) {}

void TestRunner::load_initial_state(j initial) {
    cpu->PC   = initial["pc"].get<uint16_t>();
    cpu->SP   = initial["sp"].get<uint16_t>();
    cpu->A    = initial["a"].get<uint8_t>();
    cpu->B    = initial["b"].get<uint8_t>();
    cpu->C    = initial["c"].get<uint8_t>();
    cpu->D    = initial["d"].get<uint8_t>();
    cpu->E    = initial["e"].get<uint8_t>();
    cpu->F    = initial["f"].get<uint8_t>();
    cpu->H    = initial["h"].get<uint8_t>();
    cpu->L    = initial["l"].get<uint8_t>();
    cpu->_ime = initial["ime"].get<uint8_t>();
    // interrupt->_IE = initial["ie"].get<uint8_t>();

    j ram = initial["ram"];
    for (auto r : ram) {
        mmu->write8(r[0].get<uint16_t>(), r[1].get<uint8_t>());
    }
}
void TestRunner::verify_final_state(j final) {

    if (cpu->PC != final["pc"].get<uint16_t>()) {

        printf("PC mismatch: expected %04X got %04X\n", final["pc"].get<uint16_t>(), cpu->PC);
        exit(1);
    }

    if (cpu->SP != final["sp"].get<uint16_t>()) {
        printf("SP mismatch: expected %04X got %04X\n", final["sp"].get<uint16_t>(), cpu->SP);
        exit(1);
    }

    if (cpu->A != final["a"].get<uint8_t>()) {
        printf("A mismatch: expected %02X got %02X\n", final["a"].get<uint8_t>(), cpu->A);
        exit(1);
    }

    if (cpu->B != final["b"].get<uint8_t>()) {
        printf("B mismatch: expected %02X got %02X\n", final["b"].get<uint8_t>(), cpu->B);
        exit(1);
    }

    if (cpu->C != final["c"].get<uint8_t>()) {
        printf("C mismatch: expected %02X got %02X\n", final["c"].get<uint8_t>(), cpu->C);
        exit(1);
    }

    if (cpu->D != final["d"].get<uint8_t>()) {
        printf("D mismatch: expected %02X got %02X\n", final["d"].get<uint8_t>(), cpu->D);
        exit(1);
    }

    if (cpu->E != final["e"].get<uint8_t>()) {
        printf("E mismatch: expected %02X got %02X\n", final["e"].get<uint8_t>(), cpu->E);
        exit(1);
    }

    if (cpu->F != final["f"].get<uint8_t>()) {
        printf("F mismatch: expected %02X got %02X\n", final["f"].get<uint8_t>(), cpu->F);
        exit(1);
    }

    if (cpu->H != final["h"].get<uint8_t>()) {
        printf("H mismatch: expected %02X got %02X\n", final["h"].get<uint8_t>(), cpu->H);
        exit(1);
    }

    if (cpu->L != final["l"].get<uint8_t>()) {
        printf("L mismatch: expected %02X got %02X\n", final["l"].get<uint8_t>(), cpu->L);
        exit(1);
    }

    if (cpu->_ime != final["ime"].get<uint8_t>()) {
        printf("IME mismatch: expected %d got %d\n", final["ime"].get<uint8_t>(), cpu->_ime);
        exit(1);
    }

    for (const auto& r : final["ram"]) {
        uint16_t addr     = r[0].get<uint16_t>();
        uint8_t  expected = r[1].get<uint8_t>();
        uint8_t  actual   = mmu->read8(addr);

        if (actual != expected) {
            printf("RAM mismatch at %04X: expected %02X got %02X (PC=%04X)\n", addr, expected,
                   actual, cpu->PC);
            exit(1);
        }
    }
}
