#include "Engine.h"
#include <SFML/Graphics.hpp>
#include<Windows.h>
#include <iostream>
#include <iomanip>


using namespace std;

Engine::Engine(sf::RenderWindow &win, bool b)
{
	for (int i = 0; i < Level::COUNT; i++)
	{
		if (!textura[i].loadFromFile("../data/graph/tiles.png", sf::IntRect((i - 1)*level.tile_width, 0, level.tile_width, level.tile_height)))
		{
			MessageBox(NULL, "Texture of level not found", "ERROR", MB_OK);
			return;
		}
	}

	nr_levelu = 1;
	setLevel();

	for (int i = 0; i < Food::COUNT; i++)
	{
		if (!texture_food[i].loadFromFile("../data/graph/food.png", sf::IntRect((i - 1)*level.tile_width, 0, level.tile_width, level.tile_height)))
		{
			MessageBox(NULL, "Texture of food not found", "ERROR", MB_OK);
			return;
		}
	}

	if (!texture_water.loadFromFile("../data/graph/water.png"))
	{
		MessageBox(NULL, "Texture of water not found", "ERROR", MB_OK);
		return;
	}


	if (!font.loadFromFile("../data/Fonts/Teko.ttf"))
	{
		MessageBox(NULL, "Nie wczytano poprawnie czcionki", "ERROR", MB_OK);

		return;
	}



	for (int i = 0; i < Engine::COUNT; i++)
	{
		text[i].setFont(font);
		text[i].setCharacterSize(30);
		text[i].setPosition(1190, 10 + i * 25);
	}
	lifes = 2;
	text[LIFE].setString("Lifes " + std::to_string(lifes));
	text[LEVEL].setString("Level " + std::to_string(nr_levelu));
	text[TIME].setCharacterSize(50);

	foodClock.reset();
	foodClock.toggle();

	srand(time(NULL));

	audio.loadMusic(false);
	audio.loadSounds();
	audio.playMusic();
	audio.setVolume(!b);

	unmute = b;

	gameClock.reset();
	gameClock.toggle();

	players_life.setPosition(10, 10);
	players_life.setFillColor(sf::Color(255, 26, 26));

	players_thirst.setFillColor(sf::Color(0, 163, 204));
	players_thirst.setPosition(10, 70);

	end_text.setFont(font);
	end_text.setCharacterSize(50);
	end_text.setPosition(50, 10);
	lose = false;
	levelUp = false;
	pauza = false;

	pauza_text.setFont(font);
	pauza_text.setCharacterSize(300);
	pauza_text.setPosition(350, 150);
	pauza_text.setString("PAUZA");


	runEngine(win);

}


void Engine::runEngine(sf::RenderWindow &window)
{
	bool menu = false;


	while (!menu)
	{
		if (!lose && !levelUp && !pauza)
		{
			sf::Event event;

			sf::Vector2f mouse(sf::Mouse::getPosition(window));

			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
				{
					menu = true;
				}
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
				{
					pauza = true;
					audio.stopMusic();
				}

			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				player.idz(mouse);
			}
			else player.stop();



			checkCollision();
			player.update(mouse, (nr_levelu>1), pauza);

			if (player.life <= 0 || player.thirst <= 0)
			{
				lifes--;
				audio.stopMusic();
				audio.playSound(Audio::GAME_OVER, false);
				lose = true;
			}

			else if (gameClock.getElapsedTime() > sf::seconds(20))
			{
				audio.stopMusic();
				audio.playSound(Audio::LEVEL_COMPLETE, (nr_levelu == 3));
				levelUp = true;
			}

			window.clear(sf::Color(51, 20, 0));
			window.draw(player);

			text[TIME].setString(std::to_string((int)(20 - gameClock.getElapsedTime().asSeconds())));


			for (int i = 0; i < COUNT; i++)
			{
				window.draw(text[i]);
			}

			for (int i = 0; i < level.height; i++)
			{
				for (int j = 0; j < level.width; j++)
				{
					if (level.poziom[i][j].type != Level::NONE)
						window.draw(sprite[i][j]);
				}
			}

			setFood();
			if (nr_levelu > 1)
				setWater();

			for (int i = 0; i < level.height; i++)
			{
				for (int j = 0; j < level.width; j++)
				{
					if (food.jedzenie[i][j].rodzaj != Food::NONE)
						window.draw(sprite_food[i][j]);
				}
			}
			for (int i = 0; i < level.height; i++)
			{
				for (int j = 0; j < level.width; j++)
				{
					if (food.water[i][j].isWater == true)
						window.draw(sprite_water[i][j]);
				}
			}

			players_life.setSize(sf::Vector2f(player.life * 2, 50));
			if (nr_levelu > 1 && player.thirst >0)
			{
				players_thirst.setSize(sf::Vector2f(player.thirst, 20));
				window.draw(players_thirst);
			}
			window.draw(players_life);

			window.display();
		}
		if (lose)
		{
			window.clear(sf::Color(51, 20, 0));
			end_text.setString("You lose!  Press space to continue...");
			window.draw(end_text);
			for (int i = 0; i < COUNT; i++)
			{
				window.draw(text[i]);
			}

			for (int i = 0; i < level.height; i++)
			{
				for (int j = 0; j < level.width; j++)
				{
					if (level.poziom[i][j].type != Level::NONE)
						window.draw(sprite[i][j]);
				}
			}
			for (int i = 0; i < level.height; i++)
			{
				for (int j = 0; j < level.width; j++)
				{
					if (food.jedzenie[i][j].rodzaj != Food::NONE)
						window.draw(sprite_food[i][j]);
				}
			}
			for (int i = 0; i < level.height; i++)
			{
				for (int j = 0; j < level.width; j++)
				{
					if (food.water[i][j].isWater == true)
						window.draw(sprite_water[i][j]);
				}
			}
			if (nr_levelu > 1)
			{
				players_thirst.setSize(sf::Vector2f(player.thirst, 20));
				window.draw(players_thirst);
			}
			window.draw(players_life);
			window.draw(player);

			window.display();

			sf::Event event;

			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
				{
					restart();
					lose = false;
					if (lifes < 0)
					{
						menu = true;
					}
				}
				else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				{
					menu = true;
				}

			}
		}
		if (levelUp)
		{
			window.clear(sf::Color(51, 20, 0));
			if (nr_levelu < 3)
				end_text.setString("Level up!  Press space to continue...");
			else
				end_text.setString("Congratulations! You won!  Press space to continue..");
			window.draw(end_text);
			for (int i = 0; i < COUNT; i++)
			{
				window.draw(text[i]);
			}

			for (int i = 0; i < level.height; i++)
			{
				for (int j = 0; j < level.width; j++)
				{
					if (level.poziom[i][j].type != Level::NONE)
						window.draw(sprite[i][j]);
				}
			}
			for (int i = 0; i < level.height; i++)
			{
				for (int j = 0; j < level.width; j++)
				{
					if (food.jedzenie[i][j].rodzaj != Food::NONE)
						window.draw(sprite_food[i][j]);
				}
			}
			for (int i = 0; i < level.height; i++)
			{
				for (int j = 0; j < level.width; j++)
				{
					if (food.water[i][j].isWater == true)
						window.draw(sprite_water[i][j]);
				}
			}

			if (nr_levelu > 1)
			{
				players_thirst.setSize(sf::Vector2f(0, 0));
				window.draw(players_thirst);
			}

			players_life.setSize(sf::Vector2f(0, 0));
			window.draw(players_life);
			window.draw(player);

			window.display();

			sf::Event event;

			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
				{
					if (nr_levelu != 3 && lifes >= 0)
					{
						level_up();
						levelUp = false;
					}
					else menu = true;

				}
				else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				{
					menu = true;
				}

			}
		}
		if (pauza)
		{
			gameClock.pause();
			waterClock.pause();
			foodClock.pause();

			window.clear(sf::Color(51, 26, 0));

			window.draw(player);

			for (int i = 0; i < COUNT; i++)
			{
				text[i].setFillColor(sf::Color(255, 255, 255, 150));
				window.draw(text[i]);
			}

			for (int i = 0; i < level.height; i++)
			{
				for (int j = 0; j < level.width; j++)
				{
					if (level.poziom[i][j].type != Level::NONE)
					{
						sprite[i][j].setColor(sf::Color(255, 255, 255, 150));
						window.draw(sprite[i][j]);
					}

				}
			}


			for (int i = 0; i < level.height; i++)
			{
				for (int j = 0; j < level.width; j++)
				{
					if (food.jedzenie[i][j].rodzaj != Food::NONE)
					{
						sprite_food[i][j].setColor(sf::Color(255, 255, 255, 150));
						window.draw(sprite_food[i][j]);
					}

				}
			}
			for (int i = 0; i < level.height; i++)
			{
				for (int j = 0; j < level.width; j++)
				{
					if (food.water[i][j].isWater == true)
					{
						sprite_water[i][j].setColor(sf::Color(255, 255, 255, 150));
						window.draw(sprite_water[i][j]);
					}

				}
			}

			if (nr_levelu > 1 && player.thirst >0)
			{
				players_thirst.setFillColor(sf::Color(0, 163, 204, 150));
				window.draw(players_thirst);
			}

			players_life.setFillColor(sf::Color(255, 26, 26, 150));
			window.draw(players_life);
			window.draw(pauza_text);

			window.display();

			sf::Event event;

			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
				{
					pauza = false;
					audio.playMusic();
					audio.setVolume(!unmute);
					gameClock.resume();
					waterClock.resume();
					foodClock.resume();


					for (int i = 0; i < COUNT; i++)
					{
						text[i].setFillColor(sf::Color(255, 255, 255));
					}

					for (int i = 0; i < level.height; i++)
					{
						for (int j = 0; j < level.width; j++)
						{
							if (level.poziom[i][j].type != Level::NONE)
							{
								sprite[i][j].setColor(sf::Color(255, 255, 255));
							}

						}
					}


					for (int i = 0; i < level.height; i++)
					{
						for (int j = 0; j < level.width; j++)
						{
							if (food.jedzenie[i][j].rodzaj != Food::NONE)
							{
								sprite_food[i][j].setColor(sf::Color(255, 255, 255));
							}

						}
					}
					for (int i = 0; i < level.height; i++)
					{
						for (int j = 0; j < level.width; j++)
						{
							if (food.water[i][j].isWater == true)
							{
								sprite_water[i][j].setColor(sf::Color(255, 255, 255));
							}

						}
					}

					if (nr_levelu > 1 && player.thirst >0)
					{
						players_thirst.setFillColor(sf::Color(0, 163, 204));
					}

					players_life.setFillColor(sf::Color(255, 26, 26));


				}
				if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
				{
					menu = true;
					audio.playMusic();
					audio.setVolume(!unmute);
				}
			}

		}

	}
}

void Engine::checkCollision()
{
	if (player.getStatus() == STOJ) return;
	sf::FloatRect box1(player.getBoundingBox());


	box1.left += (player.getSpeed().x) / 10;
	box1.top += (player.getSpeed().y) / 10;



	/*
	Schemat sprawdzania naszego boxa z ponumerowanymi wierzcholkami
	1 ___ 2
	|     |
	3 ___ 4
	*/

	//1
	int x = box1.left / level.tile_width;
	int y = box1.top / level.tile_height;

	if (level.poziom[y][x].isWall && box1.intersects(sprite[y][x].getGlobalBounds()))
	{
		if (player.getRotation() >= 270 && player.getRotation() <= 360)
			player.stop();
	}

	else if (food.jedzenie[y][x].rodzaj && box1.intersects(sprite_food[y][x].getGlobalBounds()))
	{
		pick_up_food(x, y);
	}
	else if (nr_levelu>1 && food.water[x][y].isWater && box1.intersects(sprite_water[y][x].getGlobalBounds()))
	{
		pick_up_water(x, y);
	}
	//2
	x = (box1.left) / level.tile_width;
	y = box1.top / level.tile_height;
	if (level.poziom[y][x].isWall && box1.intersects(sprite[y][x].getGlobalBounds()))
	{
		if (player.getRotation() >= 270 && player.getRotation() <= 90)
			player.stop();
	}
	else if (food.jedzenie[y][x].rodzaj && box1.intersects(sprite_food[y][x].getGlobalBounds()))
	{
		pick_up_food(x, y);
	}
	else if (nr_levelu>1 && food.water[y][x].isWater && box1.intersects(sprite_water[y][x].getGlobalBounds()))
	{
		pick_up_water(x, y);
	}
	//3
	x = (box1.left) / level.tile_width;
	y = (box1.top + level.tile_height) / level.tile_height;
	if (level.poziom[y][x].isWall && box1.intersects(sprite[y][x].getGlobalBounds()))
	{
		if (player.getRotation() >= 180 && player.getRotation() <= 270)
			player.stop();
	}
	else if (food.jedzenie[y][x].rodzaj && box1.intersects(sprite_food[y][x].getGlobalBounds()))
	{
		pick_up_food(x, y);
	}
	else if (nr_levelu>1 && food.water[y][x].isWater && box1.intersects(sprite_water[y][x].getGlobalBounds()))
	{
		pick_up_water(x, y);
	}
	//4
	x = (box1.left + level.tile_width) / level.tile_width;
	y = (box1.top + level.tile_height) / level.tile_height;
	if (level.poziom[y][x].isWall && box1.intersects(sprite[y][x].getGlobalBounds()))
	{
		if (player.getRotation() >= 90 && player.getRotation() <= 270)
			player.stop();
	}

	else if (food.jedzenie[y][x].rodzaj && box1.intersects(sprite_food[y][x].getGlobalBounds()))
	{
		pick_up_food(x, y);
	}
	else if (nr_levelu>1 && food.water[y][x].isWater && box1.intersects(sprite_water[y][x].getGlobalBounds()))
	{
		pick_up_water(x, y);
	}
	x = (box1.left + level.tile_width * 2) / level.tile_width;
	y = box1.top / level.tile_height;
	if (level.poziom[y][x].isWall && box1.intersects(sprite[y][x].getGlobalBounds()))
	{
		if (player.getRotation() >= 0 && player.getRotation() <= 90)
			player.stop();
	}

	else if (food.jedzenie[y][x].rodzaj && box1.intersects(sprite_food[y][x].getGlobalBounds()))
	{
		pick_up_food(x, y);
	}
	else if (nr_levelu>1 && food.water[y][x].isWater && box1.intersects(sprite_water[y][x].getGlobalBounds()))
	{
		pick_up_water(x, y);
	}

	x = (box1.left + level.tile_width * 2) / level.tile_width;
	y = (box1.top + level.tile_height * 2) / level.tile_height;
	if (level.poziom[y][x].isWall && box1.intersects(sprite[y][x].getGlobalBounds()))
	{
		if (player.getRotation() >= 90 && player.getRotation() <= 180)
			player.stop();
	}

	else if (food.jedzenie[y][x].rodzaj && box1.intersects(sprite_food[y][x].getGlobalBounds()))
	{
		pick_up_food(x, y);
	}
	else if (nr_levelu>1 && food.water[y][x].isWater && box1.intersects(sprite_water[y][x].getGlobalBounds()))
	{
		pick_up_water(x, y);
	}
}
void Engine::setFood()
{


	if (foodClock.getElapsedTime() >  sf::seconds(.5f))
	{
		int x;
		int y;
		do
		{
			x = std::rand() % Level::height;
			y = std::rand() % Level::width;
		} while (level.poziom[x][y].isWall);

		food.addFood(x, y, std::rand() % Food::COUNT);
		foodClock.reset();
		foodClock.toggle();
	}

	food.removeFood();

	for (int i = 0; i < level.height; i++)
	{
		for (int j = 0; j < level.width; j++)
		{
			if (food.jedzenie[i][j].rodzaj != Food::NONE)
			{
				sprite_food[i][j].setTexture(texture_food[food.jedzenie[i][j].rodzaj]);
				sprite_food[i][j].setPosition(j*level.tile_width, i*level.tile_height);
				if (nr_levelu == 3 && food.jedzenie[i][j].rodzaj == Food::stek)
					sprite_food[i][j].setColor(sf::Color::Red);
			}
		}
	}



}
void Engine::setWater()
{

	if (waterClock.getElapsedTime() >  sf::seconds(2))
	{
		int x;
		int y;
		do
		{
			x = std::rand() % Level::height;
			y = std::rand() % Level::width;
		} while (level.poziom[x][y].isWall || food.jedzenie[x][y].rodzaj != Food::NONE);

		food.addWater(x, y);
		waterClock.reset();
		waterClock.toggle();
	}

	food.removeWater();

	for (int i = 0; i < level.height; i++)
	{
		for (int j = 0; j < level.width; j++)
		{
			if (food.water[i][j].isWater == true)
			{
				sprite_water[i][j].setTexture(texture_water);
				sprite_water[i][j].setScale(0.6, 0.4);
				sprite_water[i][j].setPosition(j*level.tile_width, i*level.tile_height);
			}
		}
	}



}

void Engine::restart()
{
	player.restart();
	audio.stopMusic();
	audio.playMusic();
	audio.setVolume(!unmute);
	food.removeAllFood();
	food.removeAllWater();
	gameClock.reset();
	gameClock.toggle();
	text[LIFE].setString("Lifes " + std::to_string(lifes));
	text[LEVEL].setString("Level " + std::to_string(nr_levelu));
	waterClock.reset();
	waterClock.toggle();

}

void Engine::setLevel()
{
	switch (nr_levelu)
	{
	case 1: level.loadFromFile("mapLevel1.txt"); break;
	case 2: level.loadFromFile("mapLevel2.txt"); break;
	case 3: level.loadFromFile("mapLevel3.txt"); break;
	}

	for (int i = 0; i < level.height; i++)
	{
		for (int j = 0; j < level.width; j++)
		{
			if (level.poziom[i][j].type != Level::NONE)
			{
				sprite[i][j].setTexture(textura[level.poziom[i][j].type]);
				sprite[i][j].setPosition(j*level.tile_width, i*level.tile_height);
			}

		}
	}
}
void Engine::level_up()
{
	nr_levelu++;
	setLevel();
	restart();
}
void Engine::pick_up_water(int x, int y)
{
	if (player.thirst != 0) player.thirst += 20;
	food.removeWater(y, x);
	audio.playSound(Audio::DRINK, false);
}
void Engine::pick_up_food(int x, int y)
{
	if (nr_levelu == 3 && food.jedzenie[y][x].rodzaj == Food::stek)
	{
		player.life -= 10;
	}

	else player.life += 10;

	food.removeFood(y, x);
	audio.playSound(Audio::CRUNCH, false);
}
Engine::~Engine()
{
	audio.stopMusic();

	fstream plik("../data/config.cfg", ios::out);
	plik << unmute;
	plik.close();


}
