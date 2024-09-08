#include <cstdint>

#include "game-boy.hpp"

void GameBoy::init(std::string filename) {
    mmu = new MMU(filename);
    registers = new Registers();
    instructions = new InstructionSet(registers, mmu);

    cpu = new CPU(registers, mmu, instructions);

    emulationRunning = true;
}

GameBoy::~GameBoy() {

}

void GameBoy::run() {
    while (emulationRunning) {
        uint32_t cyclesToRun = calculateCyclesForFrame();
        cpu->emulate_cycles(cyclesToRun);
        // Render frame, update audio, etc.
    }
}

