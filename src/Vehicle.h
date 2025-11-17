#pragma once
#include "Crosser.h"

// Declaración adelantada, solo usamos Game*
class Game;

class Vehicle : public Crosser {
public:
    // Constructor: g, tex, x, y, vx, leftSpan, rightSpan
    Vehicle(Game* g, Texture* tex,
        float x, float y,
        float vx,
        float leftSpan, float rightSpan)
        : Crosser(g, tex,
            x, y,
            (float)tex->getFrameWidth(),
            (float)tex->getFrameHeight(),
            Vector2D<>{vx, 0.0f},
            leftSpan, rightSpan)
    {
    }

    ~Vehicle() override = default;

    void render() const override {
        SceneObject::render();
    }

    Collision checkCollision(const SDL_FRect& other) const override {
        SDL_FRect me = getRect();
        if (SDL_HasRectIntersectionFloat(&me, &other))
            return { Collision::Type::ENEMY, {} };
        return {};
    }
};
