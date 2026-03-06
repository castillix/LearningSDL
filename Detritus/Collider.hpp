#pragma once

#include "Globals.hpp"

class Collider {
public:
};

class Hitbox {
public:
        bool overlapping(Hitbox other);

private:
        Point location;
};