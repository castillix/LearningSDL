#include "Snake.hpp"
#include "Random.hpp"

Snake::Snake(int startX, int startY, Grid& grid):
	head{new SnakeSegment{startX, startY, nullptr}} {
		createFood(grid);
		createFood(grid);
		createFood(grid);
		createFood(grid);
		createFood(grid);
	}

Snake::~Snake() {
	SnakeSegment* current = head;

	while(current != nullptr) {
		SnakeSegment* next = current->nextSegment;
		delete current;
		current = next;
	}

	head = nullptr;
	length = 0;
	direction = 0;
}

void Snake::setDirection(int dir) {
	if(!movementQueued) {
		direction = dir;
		movementQueued = true;
	} else {
		nextDir = dir;
	}
}

int Snake::advance(Grid& grid) {
	int prevX = head->x;
	int prevY = head->y;

	if(!movementQueued && nextDir != -1) {
		direction = nextDir;
		nextDir = -1;
	}
	movementQueued = false;
	int newX = ((direction == 1) ? prevX + 1 : (direction == 3) ? prevX - 1 : prevX) % kGridWidth;
	int newY = ((direction == 0) ? prevY + 1 : (direction == 2) ? prevY - 1 : prevY) % kGridHeight;
	newX = (newX < 0) ? kGridWidth - 1 : newX;
	newY = (newY < 0) ? kGridHeight - 1 : newY;
	bool grew = false;

	if(newX < 0 || newX >= kGridWidth || newY < 0 || newY >= kGridHeight) return 1;
	if(grid.get(newX, newY) == 1) return 1;


	if(grid.get(newX, newY) == 2) {
		length++;
		createFood(grid);
		grew = true;
	}

	head = new SnakeSegment{newX, newY, head};
	grid.set(newX, newY, 1);


	if(!grew) {
		SnakeSegment* curPos = head;
		while(curPos->nextSegment != nullptr && curPos->nextSegment->nextSegment != nullptr) {
			curPos = curPos->nextSegment;
		}
		SnakeSegment* tail = curPos->nextSegment;

		grid.set(tail->x, tail->y, 0);
		delete tail;
		curPos->nextSegment = nullptr;
	}

	return 0;
}

void Snake::createFood(Grid& grid) {
	bool placed = false;
	while(!placed) {
		int foodX = Random::get(0, kGridWidth - 1);
		int foodY = Random::get(0, kGridHeight - 1);

		if(grid.get(foodX, foodY) == 0) {
			grid.set(foodX, foodY, 2);
			placed = true;
		}
	}
}