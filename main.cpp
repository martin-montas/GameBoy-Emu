#include <iostream>
#include <SFML/Graphics.hpp>

#include "src/game-boy.hpp" 

using namespace std;

/*
int main() {
    // WHERE TO START:
    // Create a window with dimensions 800x600
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

    // Set up a circle shape with radius 50
    sf::CircleShape shape(50);
    shape.setFillColor(sf::Color::Green);

    // Main loop: run the program as long as the window is open
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window if the close button is pressed
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear the window with a black color
        window.clear(sf::Color::Black);

        // Draw the shape
        window.draw(shape);

        // Display the contents of the window
        window.display();
    }

    return 0;
}
*/
int main(int argc, char **argv) { 
    if (argc != 2) {
        cout << "Usage: GAME <ROM file>" << endl;
        return 1;
    }

    // pass the ROM file path to the gameboy constructor
    
    GameBoy *game = new GameBoy();
    game->load_rom(argv[1]);
    return 0;
}
