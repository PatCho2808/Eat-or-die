#pragma once
#include<string>
#include<fstream>


class Level
{


public:
	enum TileType
	{
		NONE = 0,
		STONE = 1,
		DIRT = 2,
		COUNT = 3
	};

	struct Tile
	{
		TileType type;
		bool isWall;
	};

	const static int height = 18, width = 32;
	const static int tile_height = 40, tile_width = 40;

	Tile poziom[height][width];

	Level();
	Level(std::string filename);
	void loadFromFile(std::string filename);
	~Level();
};

