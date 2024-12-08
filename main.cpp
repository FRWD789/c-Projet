#include <SFML/Graphics.hpp>
#include "Game.h"

int main() {
    sf::Font font;
    if (!font.loadFromFile("Roboto-Regular.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }
    Game atcSim(font);
    atcSim.run();
    return 0;

    return 0;
}