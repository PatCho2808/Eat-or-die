#include "Game.h"
#include<Windows.h>

RenderWindow window(VideoMode(1280, 720), "Eat or Die", Style::Close);

Game::Game()
{
	state = END;

	if (!font.loadFromFile("../data/Fonts/Teko.ttf"))
	{
		MessageBox(NULL, "Nie wczytano poprawnie czcionki", "ERROR", MB_OK);

		return;
	}

	state = MENU;

	audio.loadMusic(true);
	audio.loadSounds();
	audio.playMusic();

	specialClock.restart();


}

void Game::menu()
{

	Text title("Eat or Die", font, 80);
	title.setStyle(Text::Bold);

	title.setPosition(1280 / 2 - title.getGlobalBounds().width / 2, 20);

	const int ile = 2;

	Text options[ile];

	string str[ile] = { "Play","Exit" };

	for (int i = 0; i < ile; i++)
	{
		options[i].setFont(font);
		options[i].setString(str[i]);
		options[i].setCharacterSize(65);
		options[i].setPosition(1280 / 2 - options[i].getGlobalBounds().width / 2, 250 + i * 120);
	}

	bool b;

	fstream plik;
	plik.open("../data/config.cfg");
	if (plik.is_open())
		plik >> b;
	else b = true;
	plik.close();

	Text option;
	string s = "Music & Sounds ";
	if (b) option.setString(s + "ON");
	else option.setString(s + "OFF");

	option.setFont(font);
	option.setCharacterSize(20);
	option.setPosition(10, 10);
	audio.setVolume(!b);



	while (state == MENU)
	{
		Vector2f mouse(Mouse::getPosition(window));
		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed || event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
			{
				state = END;
			}


			else if (options[1].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				state = END;
			}

			else if (option.getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				if (b)
				{
					b = false;
					option.setString(s + "OFF");
				}
				else
				{
					b = true;
					option.setString(s + "ON");
				}
				audio.setVolume(!b);
				audio.playSound(Audio::CLICK, false);
			}

			else if (options[0].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				state = GAME;
				audio.stopMusic();
				audio.playSound(Audio::CLICK, false);

			}

			for (int i = 0; i < ile; i++)
			{
				if (options[i].getGlobalBounds().contains(mouse))
				{
					if (options[i].getFillColor() == sf::Color::White)
						audio.playSound(Audio::CLICK, false);
					options[i].setFillColor(Color::Blue);

				}

				else options[i].setFillColor(Color::White);
			}
		}

		window.clear();

		window.draw(title);
		for (int i = 0; i < ile; i++)
		{
			window.draw(options[i]);
		}

		window.draw(option);
		window.display();
	}
	unmute = b;
}

void Game::runGame()
{
	while (state != END)
	{
		switch (state)
		{
		case MENU:
			menu();
			break;

		case GAME:
			single();
			break;
		}
	}
}

void Game::single()
{

	Engine engine(window, unmute);

	audio.playMusic();
	audio.loadMusic(true);

	state = MENU;

}
Game::~Game()
{
	audio.stopMusic();
	fstream plik("../data/config.cfg", ios::out);
	plik << unmute;
	plik.close();

}
