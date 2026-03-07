#pragma once

#include "Globals.hpp"

class Engine {
public:
	Engine();
	~Engine();

	Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;

	bool init();
	void run();

private:
};