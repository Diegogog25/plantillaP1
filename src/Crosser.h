#pragma once
#include "SceneObject.h"
#include "vector2D.h"

class Crosser : public SceneObject
{
protected:
    Vector2D<> vel;
    float leftLimit;
    float rightLimit;

public:
    Crosser(Game* g, Texture* tex,
        float X, float Y,
        float W, float H,
        Vector2D<> v,
        float left, float right)
        : SceneObject(g, tex, X, Y, W, H),
        vel(v),
        leftLimit(left),
        rightLimit(right)
    {
    }

    ~Crosser() override = default;

    void update() override
    {
        x += vel.getX();
        y += vel.getY();

        if (vel.getX() > 0 && x > rightLimit)
            x = leftLimit;
        else if (vel.getX() < 0 && x + w < leftLimit)
            x = rightLimit;
    }

    const Vector2D<>& getVel() const { return vel; }
};
