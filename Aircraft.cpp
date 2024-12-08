#include "Aircraft.h"








Aircraft::Aircraft(std::string id, sf::Vector2f startPosition, AircraftType type, float cap, int fl, float sp, sf::Font* font):flightId(id), position(startPosition),type(type),shape(sf::Vector2f(10,10)), altitude(fl*100),_speed(sp), tail(sf::LinesStrip, 2),heading(cap),_font(font)
{
	targetHeading = heading;
	targetAltitude = altitude;
	currentWaypoint = nullptr;
	shape.setOutlineThickness(1);
	shape.setOrigin(5.0f, 5.0f);
	shape.setPosition(position);
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor((type == AircraftType::Landing ? sf::Color::Magenta : sf::Color::Blue));
	//font et text pour les icon avion
	std::string msg = id + "\n" + std::to_string(altitude) + "FT";
	//std::cout << msg;
	label.setFont(*_font);
	label.setString(msg);
	std::cout << label.getString().toAnsiString();
	label.setCharacterSize(12);
	label.setFillColor(sf::Color::White);
	label.setPosition(shape.getOrigin().x-5, shape.getOrigin().y + 15);
	tail[0].position = position;
	tail[0].color = sf::Color::White;
}

std::string Aircraft::getflightId() const
{
    return this->flightId;
}

sf::Vector2f Aircraft::getPosition() const
{
    return this->position;
}

float Aircraft::getHeading() const
{
    return this->heading;
}

int Aircraft::getAltitude() const
{
    return this->altitude;
}

AircraftType Aircraft::getType() const
{
    return this->type;
}

void Aircraft::setHeading(float newHeading)
{


	heading = newHeading;
	
	
}

void Aircraft::updateHeading() {
	if (currentWaypoint) {
		sf::Vector2f direction = *currentWaypoint - position;
		int angle = (int)(atan2(direction.y, direction.x) * 180.0f /PI);
		if (angle < 0) {
			angle += 360;
		}
		setHeading(angle);
	}
}
void Aircraft::turnLeft(int degrees) {
	int newheading = targetHeading -  degrees;
	if (newheading < 0) newheading += 360;
	targetHeading = newheading;
}

void Aircraft::turnRight(int degrees)
{
	int newheading = targetHeading + degrees;
	if (newheading >= 360) newheading -= 360;
	targetHeading = newheading;

}


bool Aircraft::isOutOfAirspace() const
{
	return position.x < 0 || position.x >  1000 || position.y < 0 || position.y > 800;
}


void Aircraft::setAltitude(int newAltitude) {
	try
	{
		if (newAltitude*100 < 1000 ) {
			throw std::out_of_range("Altitude must be above 9999");
		}
		altitude = newAltitude;
	}
	catch (const std::out_of_range& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;


	}
}

void Aircraft::setFlightLevel(float fl)
{
	try
	{
		if (fl < 10) {
			throw std::out_of_range("Altitude must be above 9999");
		}
		altitude = fl*100;
	}
	catch (const std::out_of_range& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;


	}
}

void Aircraft::setTargetFl(float flightLevel)
{
	targetAltitude = flightLevel * 100;
}



void Aircraft::setPosition(sf::Vector2f newPosition)
{
	position = newPosition;
	shape.setPosition(position);
}

void Aircraft::updatePosition(float deltaTime) {
//speed = dx/dt ,x(teta) = speed*dt*cos(teta) y(teta) = speed*dt*sin(teta) 
	float speed = 2.f; // Speed in pixels per second

	std::string cliMsg = flightId + "\n" + std::to_string(int(heading))+ "°\n" + std::to_string(altitude) + "FT";
	label.setFillColor(sf::Color::White);
	turnTo(targetHeading, deltaTime);
	if (currentWaypoint) {
		// calcule de la direction
		sf::Vector2f direction = *currentWaypoint - position;
		float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

		//// gnere un veteur unitaire direction vector
		if (distance != 0) {
			direction /= distance;
		}
		if (altitude > targetAltitude) {
			cliMsg += "⇓" + std::to_string(altitude) + "FT";
			label.setFillColor(sf::Color::Red);
			float descentDistance = altitude - targetAltitude;
			float timeToReach = distance / speed;
			float descentRate = descentDistance / timeToReach; 
			altitude -= descentRate * deltaTime;  
			if (altitude < targetAltitude) altitude = targetAltitude;
		}
		if (altitude < targetAltitude) {
			cliMsg += "⇑" + std::to_string(targetAltitude) + "FT";
			label.setFillColor(sf::Color::Green);
			float descentDistance = targetAltitude - altitude;
			float timeToReach = distance / speed; 
			float descentRate = descentDistance / timeToReach; 
			altitude += descentRate * deltaTime;
			if (altitude > targetAltitude) altitude = targetAltitude;
		}




	}
	if (altitude < static_cast<int>(targetAltitude)) {
		cliMsg += "⇑" + std::to_string(static_cast<int>(round((targetAltitude)))) + "FT";
		label.setFillColor(sf::Color::Green);
		altitude += static_cast<int>(speed * deltaTime * 500); // Climb
		if (altitude > targetAltitude) altitude = targetAltitude;
	}
	if (altitude > targetAltitude) {
		cliMsg += "⇓" + std::to_string(static_cast<int>(round((targetAltitude)))) + "FT";
		label.setFillColor(sf::Color::Red);
		altitude -= static_cast<int>(speed * deltaTime * 500); // Descend
		if (altitude < targetAltitude) altitude = targetAltitude;
	}




	position.x += speed * deltaTime * cos(toRadians(heading));
	position.y += speed * deltaTime * sin(toRadians(heading));

	float tailLength = 50.0f;
	sf::Vector2f tailDirection = sf::Vector2f(cos(toRadians(heading)), sin(toRadians(heading))) * tailLength;
	tail[0].position = position;
	tail[1].position = position - tailDirection;
	tail[0].color = sf::Color::White;
	tail[1].color = sf::Color::White;
	label.setString(cliMsg);
	label.setPosition(position.x-5 , position.y + 15);
	shape.setPosition(position);
}

void Aircraft::directTo(sf::Vector2f waypoint)
{
	currentWaypoint = &waypoint;
	updateHeading();
}
bool Aircraft::checkCollision(const Aircraft& other)  {
	float dx = position.x - other.position.x;
	float dy = position.y - other.position.y;
	float distance = std::sqrt(dx * dx + dy * dy);
	float collisionRadius = 20.0f;
	return distance < collisionRadius && other.altitude == this->altitude;
}
bool Aircraft::isLanding(Runway& r) const
{
	const float errorMargin = 10.0f;
	const float dMax = 100.0f;
	sf::Vector2f directionToRunway = r.getPosition() - position;
	float distanceToRunway = std::sqrt(directionToRunway.x * directionToRunway.x + directionToRunway.y * directionToRunway.y);
	
	bool descendingToRunway = altitude > r.getElevation() && targetAltitude <=2000;
	bool LandingRange = distanceToRunway <= dMax;
	return   descendingToRunway && LandingRange; 
}
void Aircraft::turnTo(float targetHeading, float deltaTime) {

	targetHeading = fmod(targetHeading, 360.0f);// convertion en int;
	if (targetHeading < 0) {
		targetHeading += 360.0f;
	}
	float diff = targetHeading - heading; // soit tourner L soit R
	if (diff > 180.0f) {
		diff -= 360.0f;  // sense trigo
	}
	else if (diff < -180.0f) {
		diff += 360.0f;  // sense inverse trigo
	}


	float turnRate = 10.0f;
	if (abs(diff) > 1.0f) {  
	
		if (diff > 0) {
			heading += turnRate * deltaTime;
			if (heading > targetHeading) heading = targetHeading; 
		}
		else {
			heading -= turnRate * deltaTime;
			if (heading < targetHeading) heading = targetHeading; 
		}
	}

	
	
}
void Aircraft::draw(sf::RenderWindow& window)  {
	
	window.draw(shape);
	window.draw(tail);
	window.draw(label);
	


}

bool Aircraft::operator==(const Aircraft& rhs)
{
	return this->getflightId()==rhs.getflightId() && this->getType() == rhs.getType();
}

bool Aircraft::isClicked(const sf::Vector2f& mousePos)
{
	return shape.getGlobalBounds().contains(mousePos);
}


