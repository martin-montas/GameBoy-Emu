// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#include "src/gameboy.hpp"
#include <iostream>
#include <SDL2/SDL.h>
// #include "src/test/test-runner.hpp"

int main(int argc, char** argv) {
    GameBoy* game = new GameBoy(argv[1]);
    game->run();
    delete (game);

    // for uint tests:

    // TestRunner test;
    // test.run_cpu_test(argv[1]);

    return 0;
}
