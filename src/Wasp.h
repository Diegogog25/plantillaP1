#pragma once
#include "texture.h"
#include "vector2D.h"
#include "SDL3/SDL.h"
#include "collision.h"

class Wasp
{
    Texture* texture = nullptr;
    Point2D   pos;
    Vector2D<> vel;
    unsigned long long expireAtMs = 0; // tiempo de caducidad en ms

public:
    // lifetimeMs: tiempo de vida en milisegundos
    Wasp(Texture* t, Point2D p, Uint32 lifetimeMs);

    void render() const;
    void update();
    bool isAlive() const;
    Collision checkCollision(const SDL_FRect& rect) const;

    const Point2D& getPos() const { return pos; }
    const Vector2D<>& getVel() const { return vel; }
};
