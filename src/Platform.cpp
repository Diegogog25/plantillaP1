#pragma once
#include "Platform.h"

    Platform::Platform(Game* g, Texture* tex,
        Point2D pos,
        float W, float H,
        Vector2D<> v,
        float left, float right) : Crosser(g, tex, pos, W, H, v, left, right){ }

    Platform::~Platform() {}

    Collision Platform::checkCollision(const SDL_FRect& frog)  {
        SDL_FRect me = getRect();
        if (SDL_HasRectIntersectionFloat(&me, &frog))
            return { Collision::Type::PLATFORM, vel };
        return{};
    }

