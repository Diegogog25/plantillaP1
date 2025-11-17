#pragma once
#include "vector2D.h"
#include "texture.h"
#include "SDL3/SDL.h"
#include "collision.h"

// Clase base para troncos y grupos de tortugas
class Platform
{
protected:
    Texture* texture = nullptr;
    Point2D   pos;
    Vector2D<> vel;

public:
    Platform(Texture* tex, Point2D p, Vector2D<> v);
    virtual ~Platform();

    virtual void render() const;
    virtual void update();
    virtual Collision checkCollision(const SDL_FRect& rect) const;

    const Vector2D<>& getVel() const { return vel; }
    const Point2D& getPos() const { return pos; }
};
