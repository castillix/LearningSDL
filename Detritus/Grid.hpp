#pragma once

#include <vector>
#include "Globals.hpp"

class Grid {
public:
	Grid(int width, int height);

	Tile get(int x, int y);
	void set(int x, int y, Tile value);

private:
	int mWidth;
	int mHeight;
	std::vector<Tile> mData;
};

class Tile {
	
};