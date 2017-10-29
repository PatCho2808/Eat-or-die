#pragma once

#include <SFML/Graphics.hpp>
#include"Level.h"


using namespace sf;

class Food
{

	float live;
public:
	Food();
	~Food();

	enum Rodzaje_jedzenia
	{
		NONE, tomato, cheese, lemon,
		carrot, stek, grapes, COUNT
	};

	struct Foods
	{
		Rodzaje_jedzenia rodzaj;
		sf::Clock clock;
	};

	struct Water
	{
		bool isWater;
		sf::Clock clock;
	};

	Foods jedzenie[Level::height][Level::width];
	Water water[Level::height][Level::width];

	void addFood(int, int, int);
	void removeFood();
	void removeFood(int, int);
	void removeAllFood();

	void addWater(int, int);
	void removeWater();
	void removeWater(int, int);
	void removeAllWater();

	int points;
};

