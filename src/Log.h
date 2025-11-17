#pragma once
#include "Platform.h"

// Declaración adelantada opcional
class Game;

class Log : public Platform {
public:
    // Constructor: g, tex, x, y, vx, leftSpan, rightSpan
    Log(Game* g, Texture* tex,
        float x, float y,
        float vx,
        float leftSpan, float rightSpan)
        : Platform(g, tex,
            x, y,
            (float)tex->getFrameWidth(),
            (float)tex->getFrameHeight(),
            Vector2D<>{vx, 0.0f},
            leftSpan, rightSpan)
    {
    }

    ~Log() override = default;
};
