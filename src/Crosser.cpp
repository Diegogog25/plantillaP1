#include "Crosser.h"
#include "game.h"
#include "Texture.h"

Crosser::Crosser(Game* g, Texture* tex, //constructor
    Point2D pos,
    float W, float H,
    Vector2D<> v,
    float left, float right)
    : SceneObject(g, tex, pos, W, H),
    vel(v),
    leftLimit(left),
    rightLimit(right)
{
}

Crosser::~Crosser() {} // destructor

void Crosser::update() // actualización de posición y límites
{
    pos += vel;

    if (vel.getX() > 0 && pos.getX() > rightLimit)
        pos.setX(leftLimit);
    else if (vel.getX() < 0 && pos.getX() + w < leftLimit)
        pos.setX(rightLimit);
}

const Vector2D<>& Crosser::getVel() const // obtener velocidad
{
    return vel;
}