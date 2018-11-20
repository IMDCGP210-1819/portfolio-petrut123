#pragma once

/**
 * BaseEntity, the basic building block of any AI agent in the framework.
 * 
 * This is the parent class for any agents desired to be added to the system,
 * it inherits from sf::Transformable to give us access to set / get position and related functionality.
 */

#include <string>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

// we need to tell the compiler that it's ok to use the default defines from math.h, if we don't, we don't get access to M_PI!
#define _USE_MATH_DEFINES

#include <math.h>

class BaseEntity : public sf::Transformable
{
public:
	/**
	 * static std vector of BaseEntity pointers, currently used to loop all entities for rendering and ticking.
	 */
	static std::vector<BaseEntity*> Renderables;
public:
	/**
	 * Base constructor, defaults all variables.
	 */
	explicit BaseEntity();
	/**
	 * Constructor that accepts a texture file name
	 * @param file a string filename
	 */
	explicit BaseEntity(std::string file);
	/**
	 * Constructor that accepts a texture file name and a colour tint
	 * @param file a string filename
	 * @param colour a sf::Color value
	 */
	explicit BaseEntity(std::string file, sf::Color colour);

	/**
	 * Deconstructor
	 */
	virtual ~BaseEntity();

	/** 
	 * Think is the standard "update" function for all agents
	 * All agents must implement their own Think function - there is nothing provided in the base class.
	 */
	virtual void Think(sf::RenderWindow &window);

	/**
	 * GetSprite provides access to the entities sprite object
	 * @return sf::Sprite current entities sprite
	 */
	sf::Sprite GetSprite() { return sprite; }

	/**
	 * SetColour allows for post-construction alteration of the colour tint
	 * @param colour a sf::Colour value
	 */
	void SetColour(sf::Color colour) { colourTint = colour; }

	/**
	* SetTextureFile is used to set the filename variable
	* @param string a filename
	*/
	void SetTextureFile(std::string file) { filename = file; }

	const sf::Vector2f GetVelocity() const {
		return velocity;
	}

	void FlockBehaviour() 
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
	}

	void SeekFleeBehaviour(sf::RenderWindow &window, bool seek)
	{
		sf::Vector2f desiredVelocity;
		sf::Vector2f steeringVector;

		sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

		if (seek)
		{
			desiredVelocity.x = mousePosition.x - this->getPosition().x;
			desiredVelocity.y = mousePosition.y - this->getPosition().y;
		}
		else
		{
			desiredVelocity.x = this->getPosition().x - mousePosition.x;
			desiredVelocity.y = this->getPosition().y - mousePosition.y;
		}
		

		//Normalization
		desiredVelocity = desiredVelocity / std::sqrt(desiredVelocity.x * desiredVelocity.x + desiredVelocity.y * desiredVelocity.y);
		desiredVelocity *= 0.04f;

		steeringVector = desiredVelocity - velocity;

		velocity += steeringVector;

		//Normalization
		velocity = velocity / std::sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));
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

protected:
	std::string filename;
	sf::Sprite sprite;
	sf::Texture texture;
	sf::Color colourTint;

	sf::Vector2f velocity;

protected:
	/**
	 * Initialize assures our texture is loaded using the assigned file name and initial colour tint
	 */
	virtual void Initialize();
};


