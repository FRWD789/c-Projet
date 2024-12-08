#pragma once
#include <SFML/Graphics.hpp>
#include<queue>
#include<iostream>
#include"LinkedList.h"
#include"Runway.h"
#include"Aircraft.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT  600
#define  CENTER_X  WINDOW_WIDTH / 2
#define CENTER_Y  WINDOW_HEIGHT / 2
#define SPAWN_INTERVAL 5.0f
class Game
{
    bool gameOver = false;
    float spawnInterval = SPAWN_INTERVAL;
    int score = 0; 
    int collisions = 3; 
	sf::RenderWindow window;
    sf::Font _font;
    sf::Clock spawnClock;
	Runway runway;
    sf::Text commandText;
    sf::Text queueText;
    sf::Text landedPlane;
    sf::Text nbLive;
	LinkedList<Aircraft> aircraftList;
    std::string currentCommand;
	std::queue<std::string> commandQueue;
	Aircraft* selectedAircraft = nullptr;
    std::vector<sf::CircleShape> nauticalMiles;

    void handleEvents();
    
    sf::CircleShape generateCircle(float radius, sf::Color color);
    void handleMouseClick(int x, int y);
    void proccessCommand(Aircraft* obj, std::queue<std::string>& commands);
    void spawnAircraft(std::string flightNumber);

    void renderGame();
    void updateGame(float deltaTime);
    
    void handleCommandInput(sf::Uint32 unicode);
public:

	Game(sf::Font & font);
    
	void run();

};

