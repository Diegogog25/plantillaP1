#pragma once
#include "Crosser.h"

class Platform : public Crosser
{
public:
    Platform(Game* g, Texture* tex,
        Point2D pos,
        float W, float H,
        Vector2D<> v,
        float left, float right);

    ~Platform() override;
};
