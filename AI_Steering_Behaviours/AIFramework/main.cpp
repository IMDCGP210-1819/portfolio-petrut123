#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>

#include "BaseEntity.h"
#include "UtilRandom.h"

int main()
{
	// construct a window that uses a resolution of 800 x 600
	sf::RenderWindow window(sf::VideoMode(1080, 900), "AI Framework");

	// because we're running as a console application, we still get access to the console, so can use std::cout to print to it
	std::cout << "Constructed SFML Window" << std::endl;

	// construct 5 base entities
	for (int i = 0; i < 1; i++) 
	{
		BaseEntity *boid = new BaseEntity();
		boid->setOrigin(16.0f, 16.0f);
	}

	sf::Texture texture;

	texture.loadFromFile("assets\\boid.png");
	texture.setSmooth(true);
	// and provide the sprite with a pointer to the texture object
	// if the texture object is destroyed (goes out of scope etc) then the sprite will display weirdly

	sf::Sprite sprite;

	sprite.setTexture(texture);

	//sprite.setScale(0.5f, 0.5f);
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
			entity->Think(window);
		}

		// and then draw them
		for (auto entity : BaseEntity::Renderables)
		{
			window.draw(entity->GetSprite(), entity->getTransform());
		}

		sf::Vector2i mousePos = sf::Mouse::getPosition(window);

		sprite.setPosition(sf::Vector2f(mousePos.x, mousePos.y));
		window.draw(sprite);

		// swap the frame buffers
		window.display();
	}

	return 0;
}
