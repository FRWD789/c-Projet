#include "Game.h"

Game::Game(sf::Font& font):window(sf::VideoMode(1000, 800), "ATC Simulator"),runway(sf::Vector2f(CENTER_X, CENTER_Y), 180, 50, "18/36",font),_font(font)
{



    for (int i = 1; i <= 5; ++i) {
        nauticalMiles.push_back(generateCircle(i * 100, sf::Color::Color(205,223,229,125)));
        
    }
    nbLive.setFont(font);
    nbLive.setCharacterSize(16);
    nbLive.setFillColor(sf::Color::Red);
    nbLive.setPosition(WINDOW_WIDTH-150, 30);


    landedPlane.setFont(font);
    landedPlane.setCharacterSize(16);
    landedPlane.setFillColor(sf::Color::Green);
    landedPlane.setPosition(WINDOW_WIDTH - 150, 0);


    commandText.setFont(font);
    commandText.setCharacterSize(20);
    commandText.setFillColor(sf::Color::White);


    queueText.setFont(font);
    queueText.setCharacterSize(16);
    queueText.setFillColor(sf::Color::Cyan);
    queueText.setPosition(0, 30);



}

void Game::run(){
    sf::Clock clock;
    static int flightNumberCounter = 100;
    std::string flightNumber = "Flight_" + std::to_string(flightNumberCounter);
    spawnAircraft(flightNumber);
    while (window.isOpen()) {
       
        if (spawnClock.getElapsedTime().asSeconds() >= spawnInterval ) {
            flightNumberCounter--;
            if (flightNumberCounter < 0) {
                flightNumberCounter += 200;
            }
            flightNumber = "Flight_" + std::to_string(flightNumberCounter);
            spawnAircraft(flightNumber);
            spawnClock.restart();
        }
        float deltaTime = clock.restart().asSeconds();
        handleEvents();
        commandText.setString("Current Command: " + currentCommand);
        updateGame(deltaTime);
        for (int i = 1; i < 5; ++i) {
            window.draw(nauticalMiles[i]);

        }
        std::string queueContent = "Command Queue:\n";
        std::queue<std::string> tempQueue = commandQueue;
        while (!tempQueue.empty()) {
            queueContent += tempQueue.front() + "\n";
            tempQueue.pop();
        }
        queueText.setString(queueContent);
        landedPlane.setString("Landed plane: " + std::to_string(score));
        nbLive.setString("Nblive: " + std::to_string(collisions));
        renderGame();
    }
}
void Game::handleEvents()
{

    static float currentZoom = 1.0f;
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                handleMouseClick(event.mouseButton.x, event.mouseButton.y);
            }
        }

        // Handle command input
        if (event.type == sf::Event::TextEntered && selectedAircraft) {

            handleCommandInput(event.text.unicode);

        }
        if (event.type == sf::Event::MouseWheelScrolled)
        {
    
                
                if (event.mouseWheelScroll.delta > 0) // Scroll up
                {
                    float newZoom = currentZoom * 0.9f;
                    if (newZoom > 0.5f) // limit 
                    {
                        currentZoom = newZoom;
                        sf::View view = window.getView();
                        view.zoom(0.9f); // zoom in
                        window.setView(view);
                    }
                }
                if (event.mouseWheelScroll.delta < 0) // Scroll down
                {
                    float newZoom = currentZoom * 1.1f;
                    if (newZoom < 3.0f) // limit 
                    {
                        currentZoom = newZoom;
                        sf::View view = window.getView();
                        view.zoom(1.1f); // zoom out
                        window.setView(view);
                    }
                }
            
                
            
         
        }
    }
}
sf::CircleShape Game::generateCircle(float radius, sf::Color color)
{
    sf::CircleShape circle(radius);
    circle.setOrigin(radius, radius);
    circle.setPosition(CENTER_X, CENTER_Y);
    circle.setOutlineColor(color);
    circle.setOutlineThickness(2);
    circle.setFillColor(sf::Color::Transparent);
    
    return circle;
}
void Game::handleMouseClick(int x, int y)
{

    list<Aircraft>* curr = aircraftList.getHead();
    while (curr) {
        if (curr->_element.isClicked({ (float)x,(float)y })) {
            selectedAircraft = &curr->_element;
            currentCommand = selectedAircraft->getflightId();
            return;
        }
        curr = curr->_next;
    }
    selectedAircraft = nullptr;
    currentCommand.clear();
    while (!commandQueue.empty()) {
        std::cout << "Processing Command: " << commandQueue.front() << std::endl;
        commandQueue.pop();
        currentCommand.clear();
    }

}
void Game::proccessCommand(Aircraft* obj, std::queue<std::string>& commands)
{
    if (commandQueue.front() == obj->getflightId()) {

        commandQueue.pop();
        std::string action = commandQueue.front();
        if (commandQueue.front() == "T") {
            commandQueue.pop();
            if (commandQueue.front() == "L") {
                commandQueue.pop();
                obj->turnLeft(std::stoi(commandQueue.front()));
                commandQueue.pop();

            }
            else if (commandQueue.front() == "R") {
                commandQueue.pop();
                obj->turnRight(std::stoi(commandQueue.front()));
                commandQueue.pop();
            }
        }
        else if (action == "CL" || action == "DES") {
            commandQueue.pop();

            if (commandQueue.size() == 1) {
                obj->setTargetFl(std::stof(commandQueue.front()));
                commandQueue.pop();
            }
            else {
                std::cout << "Invalid command: " << action << std::endl;
                commandText.setString("Invalid command:");
            }



        }
        else {
            std::cout << "Invalid command: " << action << std::endl;
            commandText.setString("Unknown command");
        }
        selectedAircraft = nullptr;
    }
}
void Game::spawnAircraft(std::string flightNumber)
{
    float angle = (float)(rand() % 360); 
    float x = CENTER_X + (10 +rand()%700) * cos(angle * 3.14159f / 180.0f);
    float y = CENTER_Y + (10+rand()% 700) * sin(angle * 3.14159f / 180.0f);
    int fl = 10 + rand() % 110;
    float speed = 150 + rand() % 250;
    AircraftType type = (std::rand() % 2 == 0 && fl < 100 ) ? AircraftType::Landing : AircraftType::PassingBy;
    Aircraft newAircraft(flightNumber,sf::Vector2f(x,y), type,angle, fl, speed, &_font);

    aircraftList.push(newAircraft);  
}
void Game::renderGame()
{
    window.clear();
    for (auto& i : nauticalMiles) {
        window.draw(i);
        sf::Text label;
        label.setFont(_font);
        label.setString(std::to_string(static_cast<int>(i.getRadius() / 100)) + "NM"); // Convert radius to nautical miles
        label.setCharacterSize(12);
        label.setFillColor(i.getOutlineColor());
        label.setPosition(CENTER_X, CENTER_Y - i.getRadius()+20 );
        window.draw(label);
    }



    window.draw(commandText);
    window.draw(queueText);
    window.draw(nbLive);
    window.draw(landedPlane );
    // Draw all aircraft
    runway.draw(window);
    list<Aircraft>* curr = aircraftList.getHead();
    while (curr) {
        curr->_element.draw(window);
        curr = curr->_next;
    }

    // Highlight the selected aircraft
    if (selectedAircraft != nullptr) {
        selectedAircraft->draw(window);
    }


   
    window.draw(commandText);
    // Draw the runway
    

    window.display();
}



void Game::updateGame(float deltaTime)
{
    
        
        if (collisions <= 0) {
            gameOver = true;
        }


        list<Aircraft>* curr = aircraftList.getHead();
        std::vector<Aircraft> flightToRemove;

        while (curr && curr->_next) {
            if (curr->_element.checkCollision(curr->_next->_element)) {
                collisions--;
                flightToRemove.push_back(curr->_element);
                flightToRemove.push_back(curr->_next->_element);
            }
            else if (curr->_element.isOutOfAirspace() && curr->_element.getType() == AircraftType::Landing) {
                collisions--;
            }
            curr = curr->_next;
        }

        curr = aircraftList.getHead();
        while (curr) {
            if (curr->_element.isOutOfAirspace() || curr->_element.isLanding(runway)) {
                flightToRemove.push_back(curr->_element);
                if (curr->_element.isLanding(runway)) score++;
            }
            curr->_element.updatePosition(deltaTime);
            curr = curr->_next;
        }

        for (auto& aircraft : flightToRemove) {
            aircraftList.removeById(aircraft);
        }
}

void Game::handleCommandInput(sf::Uint32 unicode)
{
    if (unicode == 8) { // Backspace key
        if (!currentCommand.empty()) {
            currentCommand.pop_back();

        }
    }
    if (unicode >= 32 && unicode <= 126) {
        currentCommand += char(unicode);

    }
    if (unicode == 32) {
        if (!currentCommand.empty()) {
            std::string temp = currentCommand;
            temp.pop_back();
            int index = temp.rfind(' ');
            if (index != std::string::npos) {
                commandQueue.push(temp.substr(index + 1));
            }
            else {
                commandQueue.push(temp);
            }


        }

    }
    if (unicode == 13) { // Enter key
        std::string temp = currentCommand;
        temp.pop_back();
        int index = temp.rfind(' ');
        std::string commandVal = currentCommand.substr(index + 1);
        if (commandVal != "CL " && commandVal != "T " && commandVal != "L " && commandVal != "R " && commandVal != "DES ")
            commandQueue.push(commandVal);
        proccessCommand(selectedAircraft, commandQueue);
        while (!commandQueue.empty()) {
            std::cout << "Processing Command: " << commandQueue.front() << std::endl;
            commandQueue.pop();
            currentCommand.clear();
        }
        currentCommand.clear();
    }
}



