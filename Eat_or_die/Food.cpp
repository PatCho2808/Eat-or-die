#include "Food.h"
#include "Level.h"
#include <iostream>



Food::Food()
{
	for (int i = 0; i < Level::height; i++)
	{
		for (int j = 0; j < Level::width; j++)
		{
			jedzenie[i][j].rodzaj = NONE;
		}
	}

	for (int i = 0; i < Level::height; i++)
	{
		for (int j = 0; j < Level::width; j++)
		{
			water[i][j].isWater = false;
		}
	}

	live = 10;
	points = 5;
}

void Food::addFood(int x, int y, int type)
{
	if (jedzenie[x][y].rodzaj == 0)
	{
		jedzenie[x][y].rodzaj = (Rodzaje_jedzenia)type;
		jedzenie[x][y].clock.restart();
	}

}
void Food::removeFood()
{
	for (int i = 0; i < Level::height; i++)
	{
		for (int j = 0; j < Level::width; j++)
		{
			if (jedzenie[i][j].rodzaj != NONE && jedzenie[i][j].clock.getElapsedTime() > sf::seconds(live))
			{
				jedzenie[i][j].rodzaj = NONE;
			}

		}
	}
}

void Food::removeFood(int x, int y)
{
	jedzenie[x][y].rodzaj = NONE;
}

void Food::removeAllFood()
{
	for (int i = 0; i < Level::height; i++)
	{
		for (int j = 0; j < Level::width; j++)
		{
			jedzenie[i][j].rodzaj = NONE;
		}
	}
}

void Food::addWater(int x, int y)
{
	if (water[x][y].isWater == false)
	{
		water[x][y].isWater = true;
		water[x][y].clock.restart();
	}
}

void Food::removeWater()
{
	for (int i = 0; i < Level::height; i++)
	{
		for (int j = 0; j < Level::width; j++)
		{
			if (water[i][j].isWater != false && water[i][j].clock.getElapsedTime() > sf::seconds(live))
			{
				water[i][j].isWater = false;
			}

		}
	}
}

void Food::removeWater(int x, int y)
{
	water[x][y].isWater = false;
}

void Food::removeAllWater()
{
	for (int i = 0; i < Level::height; i++)
	{
		for (int j = 0; j < Level::width; j++)
		{
			water[i][j].isWater = false;
		}
	}

}
Food::~Food()
{
}
