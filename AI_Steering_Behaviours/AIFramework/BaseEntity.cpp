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
	

	float radius = 100.0f;
	int neighourCount = 0;

	sf::Vector2f mediumVelocity = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f mediumPosition = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f separationVector = sf::Vector2f(0.0f, 0.0f);

	for (auto neighbour : Renderables)
	{
		if (neighbour == this) continue;

		//This is the formula to see if a point (in this case the position of the neighbour) is inside the circle (the area of neighbours)
		float distance = std::sqrt(std::pow(neighbour->getPosition().x - this->getPosition().x, 2) + std::pow(neighbour->getPosition().y - this->getPosition().y, 2));
		if (distance <= radius)
		{
			sf::Vector2f weight = sf::Vector2f(0.0f, 0.0f);
			weight = neighbour->getPosition() - getPosition();
			weight = (weight / std::sqrt(weight.x * weight.x + weight.y * weight.y)) / distance;

			separationVector += weight;
			mediumVelocity += neighbour->GetVelocity();
			mediumPosition += neighbour->getPosition();
			neighourCount++;
		}
	}

	if (neighourCount > 0)
	{
		separationVector = separationVector / (float)neighourCount * -1.0f;

		//Normalization
		separationVector = separationVector / std::sqrt(separationVector.x * separationVector.x + separationVector.y * separationVector.y);

		mediumVelocity = mediumVelocity / (float)neighourCount;
		//Normalization
		mediumVelocity = mediumVelocity / abs(sqrt((mediumVelocity.x * mediumVelocity.x) + (mediumVelocity.y * mediumVelocity.y)));

		mediumPosition = mediumPosition / (float)neighourCount;

		mediumPosition = mediumPosition - getPosition();
		//Normalization
		mediumPosition = mediumPosition / abs(sqrt((mediumPosition.x * mediumPosition.x) + (mediumPosition.y * mediumPosition.y)));

		velocity += mediumVelocity + mediumPosition + separationVector;
	}
	
	//Normalization
	velocity += velocity / abs(sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y)));
	
	//Multiply by the default speed
	velocity *= 0.04f;

	float angle = atan2(velocity.y, velocity.x);
	setRotation(angle * 180 / M_PI);
	
	this->move(velocity);

	if (getPosition().x <= 0)
	{
		setPosition(sf::Vector2f(800.f, getPosition().y));
	} 
	else if (getPosition().x >= 800)
	{
		setPosition(sf::Vector2f(0.0f, getPosition().y));
	}

	if (getPosition().y <= 0)
	{
		setPosition(sf::Vector2f(getPosition().x, 600.0f));
	} 
	else if (getPosition().y >= 600)
	{
		setPosition(sf::Vector2f(getPosition().x, 0.0f));
	}
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