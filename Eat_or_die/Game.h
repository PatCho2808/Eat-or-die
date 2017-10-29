#pragma once
#include <SFML/Graphics.hpp>
#include "Engine.h"
#include "Audio.h"


using namespace std;
using namespace sf;


enum GameState
{
	MENU,
	GAME,
	GAME_OVER,
	END
};

class Game
{
	Font font;
	Audio audio;

	bool unmute;
	sf::Clock specialClock;


protected:

	GameState state;

public:
	Game();
	void menu();
	void runGame();
	void single();
	~Game();
};

