#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>  
#include <string>
#include "Runway.h"
#define PI 3.14159f 

enum class AircraftType {
	Landing,
	PassingBy
};
class Aircraft
{


	bool landed = false;


	std::string flightId; // vol id
	sf::Vector2f position;
	float _speed;
	float targetAltitude;
	float targetHeading;
	float heading;// en deg limit [000°,360°] 
	int altitude; //en Ft
	AircraftType type;
	sf::Vector2f* currentWaypoint = nullptr;
	sf::RectangleShape shape;
	sf::VertexArray tail ;  // drag tail
	sf::Text label;        // Aircraft label
	sf::Font* _font;
	inline float toRadians(float degrees) {
		return degrees * ( PI / 180.0f);
	}
	
public:
	Aircraft(std::string id, sf::Vector2f startPosition, AircraftType type, float cap, int fl,float sp, sf::Font* font);

	// Getters
	std::string getflightId() const;
	sf::Vector2f getPosition() const;
	float getHeading() const;
	int getAltitude() const;
	AircraftType getType() const;
	// Setters
	void setHeading(float newHeading);
	void setAltitude(int newAltitude);
	void setFlightLevel(float fl);
	void setTargetFl(float flightLevel);
	void setPosition(sf::Vector2f newPosition);
	//methods
	void updatePosition(float deltaTime);
	void directTo(sf::Vector2f waypoint);
	void updateHeading();
	void turnLeft(int degrees);
	void turnRight(int degrees);
	void climbTo(float flightLevel);
	
	void descendTo(float flightLevel);
	bool checkCollision(const Aircraft& other);
	bool isOutOfAirspace() const;
	bool isLanding(Runway& r) const;
	bool isClicked(const sf::Vector2f& mousePos);
	void turnTo(float targetHeading, float deltaTime);

	bool operator == (const Aircraft& rhs);
	//
	void draw(sf::RenderWindow& window) ;

};

