#include "TurtleGroup.h"
#include <SDL3/SDL.h> 

TurtleGroup::TurtleGroup(Game* g, Texture* tex,
    Point2D pos,
    float w, float h,
    float vx,
    float leftSpan, float rightSpan,
    int n, bool sink)
    : Platform(g, tex, pos, w, h, { vx, 0.0f }, leftSpan, rightSpan)
    , numTurtles(n)
    , submerging(sink)
{}

void TurtleGroup::update() {
    Platform::update();

    if (submerging) {
        timer = (timer + 1) % period;
        sunk = (timer > (period * 2) / 3);
    }
}

void TurtleGroup::render() const {
    if (submerging && sunk) return;

    for (int i = 0; i < numTurtles; ++i) {
        SDL_FRect dst{ pos.getX() + i * w, pos.getY(), w, h};
        tex->renderFrame(dst, 0, 0);
    }
}

Collision TurtleGroup::checkCollision(const SDL_FRect& other) const {
    if (submerging && sunk) return {};

    for (int i = 0; i < numTurtles; ++i) {
        SDL_FRect me{ pos.getX() + i * w, pos.getY(), w, h };

        if (SDL_HasRectIntersectionFloat(&me, &other))
            return { Collision::Type::PLATFORM, vel };
    }
    return {};
}
