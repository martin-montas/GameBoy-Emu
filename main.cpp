#include <iostream>
// #include <SDL2/SDL.h>
#include "./src/game-boy.hpp"

using namespace std;

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "Usage: GAME <ROM file>" << std::endl;
    return 1;
  }
  GameBoy *game = new GameBoy(argv[1]);
  game->run();
  return 0;
}
