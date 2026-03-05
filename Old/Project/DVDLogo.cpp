#include "DVDLogo.hpp"
#include "Globals.hpp"
#include <cstdlib>
#include <ctime>
#include <tuple>

DVDLogo::DVDLogo():
	r{0},
	g{0},
	b{0},

	x{0.f},
	xBound{0.f},
	xDir{true},

	y{0.f},
	yBound{0.f},
	yDir{true}
{}

DVDLogo::~DVDLogo() {
	destroy();
}

bool DVDLogo::init() {
	bool success{true};

	std::srand(std::time(0));

	if(!logo.loadFromFile("assets/loaded.png")) {
		SDL_Log("Unable to load png! %s\n", SDL_GetError());
	}
	// if(!logo.loadFromFile("assets/logo.bmp")) {
	// 	SDL_Log("Unable to load bmp! %s\n", SDL_GetError());
	// 	return false;
	// }

	xBound = kScreenWidth - static_cast<float>(logo.getWidth());
	yBound = kScreenHeight - static_cast<float>(logo.getHeight());
	
	x = std::rand() % static_cast<int>(xBound * 0.9f);
	y = std::rand() % static_cast<int>(yBound * 0.9f);

	return true;
}

void DVDLogo::destroy() {
	logo.destroy();
	r = 0;
	g = 0;
	b = 0;
	x = 0.f;
	y = 0.f;
	xBound = 0.f;
	yBound = 0.f;
	xDir = false;
	yDir = false;
}

void DVDLogo::advance() {
	// Move logo and update directions if needed
	x += (xDir) ? 1.5f : -1.5f;
	y += (yDir) ? 1.5f : -1.5f;

	if(x >= xBound || x <= 0.f) {
		xDir = !xDir;
		r = std::rand() % 256;
		g = std::rand() % 256;
		b = std::rand() % 256;
	}

	if(y >= yBound || y <= 0.f) {
		yDir = !yDir;
		r = std::rand() % 256;
		g = std::rand() % 256;
		b = std::rand() % 256;
	}
}

void DVDLogo::render() {
	logo.render(x, y);
}

float DVDLogo::getX() {
	return x;
}

float DVDLogo::getY() {
	return y;
}

SDL_Color DVDLogo::getRGB() {
	return {static_cast<Uint8>(r), static_cast<Uint8>(g), static_cast<Uint8>(b)};
}