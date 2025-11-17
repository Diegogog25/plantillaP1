#pragma once
#include "Platform.h"

class Game;

class Log : public Platform {
public:
    // Constructor
    Log(Game* g, Texture* tex,
        float x, float y,
        float vx,
        float leftSpan, float rightSpan);

    // Destructor
    ~Log() override;
};
