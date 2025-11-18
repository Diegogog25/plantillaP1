#pragma once
#include "Platform.h"
#include <iosfwd> // std::istream

class Game;

class Log : public Platform {
public:
    // Constructor
    Log(Game* g, Texture* tex,
        Point2D pos,
        float vx,
        float leftSpan, float rightSpan);

    static SceneObject* FromMap(Game* g, std::istream& ss, const char* path, int lineNum);

    using Platform::Platform;

    void update() override;
    Collision checkCollision(const SDL_FRect& rect) override;
    // Destructor
    ~Log() override;
};