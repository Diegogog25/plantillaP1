// Collision.h
#pragma once
#include "vector2D.h"

struct Collision {
    enum class Type { NONE, ENEMY, PLATFORM } type = Type::NONE;
    Vector2D<> platformVel{ 0.f, 0.f }; // solo se usa si type == PLATFORM
};
