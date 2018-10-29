#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>

#include "BaseEntity.h"
#include "UtilRandom.h"

int main()
{
	// construct a window that uses a resolution of 800 x 600
	sf::RenderWindow window(sf::VideoMode(800, 600), "AI Framework");

	// because we're running as a console application, we still get access to the console, so can use std::cout to print to it
	std::cout << "Constructed SFML Window" << std::endl;

	// construct 5 base entities
	for (int i = 0; i < 20; i++) 
	{
		BaseEntity *boid = new BaseEntity();
	}

	// our game loop
	while (window.isOpen())
	{
		// poll for events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// clear the window before drawing anything to it
		window.clear();
		
		// update all our entities
		for ( auto entity : BaseEntity::Renderables )
		{
			entity->Think();
			float radius = 25.0f;
			for (auto neighbour : BaseEntity::Renderables)
			{
				//This is the formula to see if a point (in this case the position of the neighbour) is inside the circle (the area of neighbours)
				if (sqrtf(powf(neighbour->getOrigin().x - entity->getOrigin().x, 2) + powf(neighbour->getOrigin().y - entity->getOrigin().y, 2)) < radius)
				{
					sf::Vector2f weight = neighbour->getPosition() - entity->getPosition();
					weight.x = weight.x / sqrtf(weight.x * weight.x + weight.y * weight.y);
					weight.y = weight.y / sqrtf(weight.x * weight.x + weight.y * weight.y);
					weight = weight / sqrtf(powf(neighbour->getOrigin().x - entity->getOrigin().x, 2) + powf(neighbour->getOrigin().y - entity->getOrigin().y, 2));
					//How do I get the velocity of the entity?
				}
			}
			
		}

		// and then draw them
		for (auto entity : BaseEntity::Renderables)
		{
			window.draw(entity->GetSprite(), entity->getTransform());
		}

		// swap the frame buffers
		window.display();
	}

	return 0;
}
