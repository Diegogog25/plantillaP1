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

    // Destructor
    ~Log() override;
};
