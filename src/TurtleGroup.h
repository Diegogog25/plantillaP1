#pragma once
#include "Platform.h"

// Declaración adelantada opcional
class Game;

class TurtleGroup : public Platform {
private:
    int  numTurtles;
    bool submerging;
    int  timer = 0;
    int  period = 180;
    bool sunk = false;

public:
    // Constructor: g, tex, x, y, w, h, vx, leftSpan, rightSpan, n, sink
    TurtleGroup(Game* g, Texture* tex,
        float x, float y,
        float w, float h,
        float vx,
        float leftSpan, float rightSpan,
        int n, bool sink)
        : Platform(g, tex, x, y, w, h,
            Vector2D<>{vx, 0.0f},
            leftSpan, rightSpan),
        numTurtles(n),
        submerging(sink)
    {
    }

    ~TurtleGroup() override = default;

    void update() override {
        Platform::update();
        if (submerging) {
            timer = (timer + 1) % period;
            sunk = (timer > (period * 2) / 3);
        }
    }

    void render() const override {
        if (submerging && sunk) return;

        for (int i = 0; i < numTurtles; ++i) {
            SDL_FRect dst{ x + i * w, y, w, h };
            tex->renderFrame(dst, 0, 0);
        }
    }

    Collision checkCollision(const SDL_FRect& other) const override {
        if (submerging && sunk) return {};

        for (int i = 0; i < numTurtles; ++i) {
            SDL_FRect me{ x + i * w, y, w, h };
            if (SDL_HasRectIntersectionFloat(&me, &other))
                return { Collision::Type::PLATFORM, vel };
        }
        return {};
    }
};
