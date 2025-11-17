#pragma once
#include "Crosser.h"

// Declaración adelantada
class Game;

class Vehicle : public Crosser {
public:
    // Constructor: g, tex, pos, vx, leftSpan, rightSpan
    Vehicle(Game* g, Texture* tex,
        Point2D pos,
        float vx,
        float leftSpan, float rightSpan);

    ~Vehicle() override;

    void render() const override;
    Collision checkCollision(const SDL_FRect& other) const override;
};

