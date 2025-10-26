#pragma once
#include "vector2D.h"
#include "texture.h"
#include "SDL3/SDL.h"
#include "collision.h"

class Vehicle
{
private:
    Texture* texture;
    Point2D pos;
    Vector2D<> vel;

public:
    Vehicle(Texture* _texture, Point2D _pos, Vector2D<> _vel);
    ~Vehicle();

    void render() const;
    void update();

    Collision checkCollision(const SDL_FRect& rect) const;

    // Accesores útiles
    const Point2D& getPos() const { return pos; }
    const Vector2D<>& getVel() const { return vel; }
    void setPos(Point2D p) { pos = p; }
};
