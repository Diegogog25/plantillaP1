#pragma once
#include "Platform.h"

class Game;

class Log : public Platform {
public:
    // Constructor
    Log(Game* g, Texture* tex,
        Point2D pos,
        float vx,
        float leftSpan, float rightSpan);

    using Platform::Platform;

    void update() override;
    Collision checkCollision(const SDL_FRect& rect) override;
    // Destructor
    ~Log() override;
};
