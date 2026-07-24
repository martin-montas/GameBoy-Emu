// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Author: Martin Montas, martinmontas1@gmail.com
#include "src/gameboy.hpp"
#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char** argv) {
    GameBoy* game = new GameBoy(argv[1]);
    game->run();
    delete (game);
    return 0;
}
