#pragma once

#include "Globals.hpp"
#include "SnakeSegment.hpp"
#include "Grid.hpp"

class Snake {
public:
	Snake(int startX, int startY, Grid& grid);
	~Snake();

	void setDirection(int dir);
	int advance(Grid& grid); // goes to next tile, handles removal of tail and eating food

private:
	int direction{0}; // 0=down, 1=right, 2=up, 3=left
	int nextDir{-1};
	int length{1};
	bool movementQueued{false};
	SnakeSegment* head{nullptr};

	void createFood(Grid& grid);
};