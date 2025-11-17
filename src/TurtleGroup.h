#pragma once

#include "Platform.h"     
#include <SDL3/SDL_rect.h> 

class Game;     
class Texture;  

class TurtleGroup : public Platform {
private:
    int  numTurtles;
    bool submerging;
    int  timer = 0;
    int  period = 180;
    bool sunk = false;

public:
    TurtleGroup(Game* g, Texture* tex,
        Point2D pos,
        float w, float h,
        float vx,
        float leftSpan, float rightSpan,
        int n, bool sink);

    ~TurtleGroup() override = default;

    void update() override;
    void render() const override;

    Collision checkCollision(const SDL_FRect& other) const override;
};
