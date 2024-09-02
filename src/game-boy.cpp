
#include <fstream>
#include <iostream>
#include <cstdint>

#include "game-boy.hpp"

GameBoy::GameBoy() {


}


GameBoy::~GameBoy() {

}


void GameBoy::run_emulation() {
    while (emulationRunning) {
        uint32_t cyclesToRun = calculateCyclesForFrame();
        cpu->emulate_cycles(cyclesToRun);
        // Render frame, update audio, etc.
    }
}

