#include "BaseEntity.h"
#include "UtilRandom.h"
#include "SFML/Window/Mouse.hpp"

std::vector<BaseEntity*> BaseEntity::Renderables;

BaseEntity::BaseEntity()
{
	// with no further information, we make some assumptions and set default values.
	filename = "assets\\boid.png";
	colourTint = sf::Color::White;
	Initialize();
}

BaseEntity::BaseEntity(std::string file) : filename(file)
{
	// with just a texture file, we default to a white tint (so no tint at all).
	colourTint = sf::Color::White;
	Initialize();
}

BaseEntity::BaseEntity(std::string file, sf::Color colour) : filename(file), colourTint(colour)
{
	Initialize();
}

BaseEntity::~BaseEntity()
{
	
}

void BaseEntity::Think()
{
	sf::Vector2f desiredVelocity;
	sf::Vector2f steeringVector;

	sf::Vector2i mousePosition = sf::Mouse::getPosition();
	desiredVelocity.x = mousePosition.x - this->getPosition().x;
	desiredVelocity.y = mousePosition.y - this->getPosition().y;

	//Normalization
	desiredVelocity = desiredVelocity / std::sqrt(desiredVelocity.x * desiredVelocity.x + desiredVelocity.y * desiredVelocity.y);
	desiredVelocity *= 0.04f;

	steeringVector = desiredVelocity - velocity;

	velocity = velocity + steeringVector;
	
	//Normalization
	velocity = velocity / abs(sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y)));
	velocity *= 0.04f;

	float angle = atan2(velocity.y, velocity.x);
	setRotation(angle * 180 / M_PI);
	
	this->move(velocity);

	if (getPosition().x <= 0)
	{
		setPosition(sf::Vector2f(1080.0f, getPosition().y));
	} 
	else if (getPosition().x >= 1080.0f)
	{
		setPosition(sf::Vector2f(0.0f, getPosition().y));
	}

	if (getPosition().y <= 0)
	{
		setPosition(sf::Vector2f(getPosition().x, 900.0f));
	} 
	else if (getPosition().y >= 900.0f)
	{
		setPosition(sf::Vector2f(getPosition().x, 0.0f));
	}
}

void BaseEntity::Initialize()
{
	// set a default position - could be randomised.
	setPosition(UtilRandom::instance()->GetRange(0.0f, 1080.0f), UtilRandom::instance()->GetRange(0.0f, 900.0f));

	// load the texture into memory
	texture.loadFromFile(filename);
	texture.setSmooth(true);
	// and provide the sprite with a pointer to the texture object
	// if the texture object is destroyed (goes out of scope etc) then the sprite will display weirdly
	sprite.setTexture(texture);
	// set up our colour tint
	sprite.setColor(colourTint);
	
	// select a random angle
	float angle = UtilRandom::instance()->GetRange(0, 361);
	// set our rotation value
	setRotation(angle);
	// and assign a velocity, we need to convert angle to radians so it plays nicely with cos and sin.
	velocity = sf::Vector2f(cos(angle * M_PI / 180), sin(angle * M_PI / 180));

	// add the entity to the list of renderables.
	Renderables.push_back(this);
}