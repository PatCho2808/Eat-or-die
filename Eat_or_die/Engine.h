#pragma once

#include "Player.h"
#include <SFML/Graphics.hpp>
#include "Level.h"
#include "Food.h"
#include "Audio.h"
#include <cstdlib>
#include <ctime>
#include <sftools/Chronometer.hpp>



class Engine
{
	Player player;
	Level level;
	Food food;
	Audio audio;
	sftools::Chronometer foodClock;
	sftools::Chronometer gameClock;
	sftools::Chronometer waterClock;
	sf::Time opoznienie;


	bool unmute;
	int lifes;
	bool lose;
	bool levelUp;
	bool pauza;


	enum Texts
	{
		LIFE, LEVEL, TIME, COUNT
	};

	sf::Font font;
	sf::Text text[COUNT];
	int nr_levelu;

public:
	Engine(sf::RenderWindow &win, bool b);
	void runEngine(sf::RenderWindow &window);

	sf::Texture textura[Level::COUNT];
	sf::Sprite sprite[Level::height][Level::width];


	sf::Texture texture_food[Food::COUNT];
	sf::Sprite sprite_food[Level::height][Level::width];

	sf::Texture texture_water;
	sf::Sprite sprite_water[Level::height][Level::width];

	sf::RectangleShape players_thirst;
	sf::RectangleShape players_life;

	sf::Text end_text;
	sf::Text pauza_text;


	void checkCollision();
	void setFood();
	void setWater();

	void setLevel();

	void restart();
	void level_up();

	void pick_up_water(int x, int y);
	void pick_up_food(int x, int y);

	~Engine();
};

