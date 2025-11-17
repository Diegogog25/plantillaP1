#include "Crosser.h"
#include "Game.h"
#include "Texture.h"

Crosser::Crosser(Game* g, Texture* tex,
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

void Crosser::update()
{
    x += vel.getX();
    y += vel.getY();

    if (vel.getX() > 0 && x > rightLimit)
        x = leftLimit;

    else if (vel.getX() < 0 && x + w < leftLimit)
        x = rightLimit;
}

const Vector2D<>& Crosser::getVel() const
{
    return vel;
}