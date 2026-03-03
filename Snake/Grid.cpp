#include "Grid.hpp"

Grid::Grid(int width, int height):
	mWidth{width},
	mHeight{height},
	mData(width * height, 0) {}

int Grid::get(int x, int y) {
	return mData[y * mWidth + x];
}

void Grid::set(int x, int y, int value) {
	mData[y * mWidth + x] = value;
}