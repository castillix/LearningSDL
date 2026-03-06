#pragma once

#include <SDL3/SDL.h>

constexpr int kScreenWidth{1280};
constexpr int kScreenHeight{720};

constexpr int kGridWidth{64};
constexpr int kGridHeight{36};

struct Point {
        int x, y;
};