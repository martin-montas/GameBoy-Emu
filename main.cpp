// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
#include "./src/gameboy.hpp"
#include <iostream>
// #include <SDL2/SDL.h>

using namespace std;

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Usage: GAME <ROM file>" << std::endl;
    return 1;
  }
  GameBoy* game = new GameBoy(argv[1]);
  game->run();
  delete (game);
  return 0;
}
