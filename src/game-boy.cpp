#include <cstdint>

#include "game-boy.hpp"

void GameBoy::init(std::string filename) {
    mmu = new MMU(filename);
    cpu = new CPU(mmu);
    instructions = new InstructionSet(cpu,mmu);

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
uint32_t GameBoy::calculateCyclesForFrame(){
    return 0;
}
