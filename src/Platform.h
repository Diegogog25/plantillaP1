#pragma once
#include "Crosser.h"

class Platform : public Crosser
{
public:

    using Crosser::Crosser;
    ~Platform();

    Collision checkCollision(const SDL_FRect&frog)  override;
protected:
    Platform(Game* g, Texture* tex,
        Point2D pos,
        float W, float H,
        Vector2D<> v,
        float left, float right);
};
