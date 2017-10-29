#pragma once

#include <SFML/Graphics.hpp>
#include <sftools/Chronometer.hpp>

enum Status
{
	STOJ, IDZ
};

class Player : public sf::Drawable, public sf::Transformable
{

	sf::Sprite sprite;
	sf::Texture texture;

	Status status;

	float speed;
	size_t frame;

	sf::Clock animClock;
	sftools::Chronometer lifeClock;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;


protected:
	int klatka_animacji;


public:
	Player();
	~Player();

	void idz(sf::Vector2f mouse);
	void stop();
	void update(sf::Vector2f mysz, bool, bool);

	Status getStatus();

	sf::Vector2f getPosition();

	float getRotation();
	sf::FloatRect getBoundingBox();
	sf::Vector2f getSpeed();

	void restart();

	int life;
	int thirst;
};

