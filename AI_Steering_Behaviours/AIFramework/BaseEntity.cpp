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

void BaseEntity::Think(sf::RenderWindow &window)
{
	//Flocking behaviour (increase the number of boids)
	//FlockBehaviour();

	//Seek behaviour (pass true for seek and false for flee)
	//SeekFleeBehaviour(window, true);

	//Arrival behaviour
	//ArrivalBehaviour(window);
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