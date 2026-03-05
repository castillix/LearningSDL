#pragma once
#include <SDL3/SDL.h>

constexpr int kScreenWidth{1280};
constexpr int kScreenHeight{720};

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;