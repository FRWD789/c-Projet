#include "Runway.h"


Runway::Runway(sf::Vector2f position, int activeHeading, float elevation, std::string number, sf::Font& font) : position(position), activeHeading(activeHeading), elevation(elevation), runwayNumber(number),shape(sf::Vector2f(100,7)),_font(font)
{
  
    shape.setOrigin(50.f,3.5f);
    shape.setPosition(position);
    shape.setFillColor(sf::Color::White);
    shape.setRotation((float)(activeHeading));
    r_label.setFont(_font);
    std::cout << runwayNumber;
    r_label.setString(number);
    r_label.setCharacterSize(18);
    r_label.setFillColor(sf::Color::Red);
    sf::Vector2f labelOffset(0.0, -30.0f); 
    r_label.setPosition(position.x+5, position.y-50);
  
}

sf::Vector2f Runway::getPosition() const
{
    return position;
}
int Runway::getActiveRunwayHeading() const {
    return activeHeading;
}

float Runway::getElevation() const
{
    return elevation;
}

void Runway::draw(sf::RenderWindow& window) const
{
    window.draw(shape);
    
    window.draw(r_label);
   
  
}
