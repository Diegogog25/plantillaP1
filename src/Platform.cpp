#pragma once
#include "Platform.h"

Platform::Platform(Game* g, Texture* tex, // constructor
        Point2D pos,
        float W, float H,
        Vector2D<> v,
        float left, float right) : Crosser(g, tex, pos, W, H, v, left, right){ }

Platform::~Platform() {} // destructor

Collision Platform::checkCollision(const SDL_FRect& frog) { // colisiones
    SDL_FRect me = getBoundingBox();
    if (SDL_HasRectIntersectionFloat(&me, &frog))
        return { Collision::Type::PLATFORM, vel };
    return{};
}

