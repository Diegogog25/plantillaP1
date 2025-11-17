#pragma once
#include "Crosser.h"

class Platform : public Crosser
{
public:
    Platform(Game* g, Texture* tex,
        float X, float Y,
        float W, float H,
        Vector2D<> v,
        float left, float right)
        : Crosser(g, tex, X, Y, W, H, v, left, right)
    {
    }

    ~Platform() override = default;

    Collision checkCollision(const SDL_FRect& o) const override {
        SDL_FRect me = getRect();
        if (SDL_HasRectIntersectionFloat(&me, &o))
            return { Collision::Type::PLATFORM, vel };
        return {};
    }
};
