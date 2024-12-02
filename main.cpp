// Copyright 2022 Robot Locomotion Group @ CSAIL. All rights reserved.
// All components of this software are licensed under the GNU License.
// Programmer: Martin Montas, martinmontas1@gmail.com
//
#include <iostream>

#include <SFML/Graphics.hpp>
#include "./src/game-boy.hpp"

using namespace std;

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Usage: GAME <ROM file>" << endl;
        return 1;
    }
    GameBoy *game = new GameBoy(argv[1]);    
    game->run();
    return 0;
}
