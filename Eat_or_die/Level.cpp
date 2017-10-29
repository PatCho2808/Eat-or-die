#include "Level.h"
#include<fstream>
#include<windows.h>

Level::Level()
{

}
Level::Level(std::string filename)
{
	loadFromFile(filename);
}

void Level::loadFromFile(std::string filename)
{
	std::fstream plik;

	plik.open("../data/levels/" + filename, std::ios::in);

	if (!plik.good())
	{
		MessageBox(NULL, "Level map not found", "ERROR", MB_OK);

		return;
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int tmp;
			plik >> tmp;

			poziom[i][j].type = static_cast<Level::TileType>(tmp);

			if (poziom[i][j].type == 0)
				poziom[i][j].isWall = false;
			else
				poziom[i][j].isWall = true;
		}
	}

	plik.close();

}

Level::~Level()
{
}
