#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include <iostream>
class Runway {
private:
    sf::Vector2f position;       // Center position of the runway or its threshold
    int activeHeading;           // Active heading of the runway (in degrees)
    float elevation;             // Elevation of the runway (in feet)
    std::string runwayNumber;    // Runway identifier (e.g., "09", "27")
    sf::Font _font;               // Font for the label
    sf::Text r_label;    
    sf::RectangleShape shape;// Text object for the runway label

public:
    Runway(sf::Vector2f position, int activeHeading, float elevation, std::string number, sf::Font &font);
    // Getters
    sf::Vector2f getPosition() const;
    int getActiveRunwayHeading() const;
    float getElevation() const;

    void draw(sf::RenderWindow& window) const;




};
