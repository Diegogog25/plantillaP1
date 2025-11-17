#pragma once
#include "Crosser.h"

// declaración adelantada para que el parámetro Game* sea válido
class Game;

class Vehicle : public Crosser {
public:
    // span izquierda/derecha se pasa desde Game (no usamos Game::WINDOW_WIDTH aquí)
    Vehicle(Game* g, Texture* tex,
        float X, float Y,
        float VX,
        float leftSpan, float rightSpan)
        : Crosser(g, tex,
            X, Y,
            (float)tex->getFrameWidth(),
            (float)tex->getFrameHeight(),
            Vector2D<>{VX, 0.f},
            leftSpan, rightSpan)
    {
    }

    void render() const override { SceneObject::render(); }

    Collision checkCollision(const SDL_FRect& other) const override {
        SDL_FRect me = getRect();
        if (SDL_HasRectIntersectionFloat(&me, &other))
            return { Collision::Type::ENEMY, {} };
        return {};
    }
};
