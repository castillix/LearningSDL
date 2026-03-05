#pragma once

#include <vector>
#include "Globals.hpp"

class Grid {
public:
	Grid(int width, int height);

	int get(int x, int y);
	void set(int x, int y, int value);

	int getScreenX(int x);
	int getScreenY(int y);

private:
	int mWidth;
	int mHeight;
	std::vector<int> mData;
};