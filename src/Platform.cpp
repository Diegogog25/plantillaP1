#pragma once
#include "Crosser.h"

class Platform : public Crosser
{
public:
    Platform(Game* g, Texture* tex,
        float X, float Y,
        float W, float H,
        Vector2D<> v,
        float left, float right);

    ~Platform() override;
};
