#pragma once

#include "Globals.hpp"

class SnakeSegment {
public:
	int x{0};
	int y{0};
	SnakeSegment* nextSegment{nullptr};
};