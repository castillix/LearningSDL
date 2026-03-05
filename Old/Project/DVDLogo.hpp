#pragma once
#include "LTexture.hpp"
#include <tuple>

class DVDLogo {
public:
	DVDLogo();

	~DVDLogo();

	bool init();
	void destroy();

	void advance();

	void render();

	float getX();
	float getY();

	SDL_Color getRGB();

private:
	LTexture logo;

	float x;
	float y;

	float xBound;
	float yBound;

	float xDir;
	float yDir;

	int r;
	int g;
	int b;
};