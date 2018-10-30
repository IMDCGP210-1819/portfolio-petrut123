#include "BaseEntity.h"
#include "UtilRandom.h"

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
	if (getPosition().x <= 0.0f || getPosition().x >= 800.0f || getPosition().y <= 0.0f || getPosition().y >= 600.0f)
	{
		setPosition(UtilRandom::instance()->GetRange(0.0f, 800.0f), UtilRandom::instance()->GetRange(0.0f, 600.0f));
	}
	float radius = 25.0f;
	
	sf::Vector2f mediumVelocity = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f mediumPosition = sf::Vector2f(0.0f, 0.0f);
	//Flock behaviour
	for (auto neighbour : Renderables)
	{
		if (neighbour == this) continue;

		//This is the formula to see if a point (in this case the position of the neighbour) is inside the circle (the area of neighbours)
		if (std::sqrt(std::pow(neighbour->getOrigin().x - this->getOrigin().x, 2) + std::pow(neighbour->getOrigin().y - this->getOrigin().y, 2)) < radius)
		{
			sf::Vector2f weight = neighbour->getPosition() - this->getPosition();
			weight.x = weight.x / std::sqrt(weight.x * weight.x + weight.y * weight.y);
			weight.y = weight.y / std::sqrt(weight.x * weight.x + weight.y * weight.y);
			weight = weight / std::sqrt(std::pow(neighbour->getPosition().x - this->getPosition().x, 2) + std::pow(neighbour->getPosition().y - this->getPosition().y, 2));
			
			mediumVelocity += neighbour->GetVelocity() + weight;
			mediumPosition += neighbour->getPosition() + weight;
		}
	}

	mediumVelocity.x = mediumVelocity.x / Renderables.size();
	mediumVelocity.y = mediumVelocity.y / Renderables.size();

	mediumPosition.x = mediumPosition.x / Renderables.size();
	mediumPosition.y = mediumPosition.y / Renderables.size();
	// build a new position vector by adding a scaled version of the velocity vector
	sf::Vector2f pos = getPosition() - mediumPosition + mediumVelocity;
	//setPosition(pos);
	this->move(pos);
}

void BaseEntity::Initialize()
{
	// set a default position - could be randomised.
	setPosition(UtilRandom::instance()->GetRange(0.0f, 800.0f), UtilRandom::instance()->GetRange(0.0f, 600.0f));

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