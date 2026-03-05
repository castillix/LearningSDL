#pragma once

#include <SDL3/SDL.h>

constexpr int kScreenWidth{1280};
constexpr int kScreenHeight{720};

constexpr int kGridWidth{64};
constexpr int kGridHeight{36};

constexpr int TILE_EMPTY{0};
constexpr int TILE_SNAKE{1};
constexpr int TILE_FOOD{2};