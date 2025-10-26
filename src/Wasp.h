#pragma once
#include "vector2D.h"
#include "texture.h"
#include "SDL3/SDL.h"
#include "Collision.h"

class Wasp
{
private:
    Texture* texture = nullptr;
    Point2D   pos;
    Vector2D<> vel;
    unsigned long long expireAtMs = 0; // SDL_GetTicksNS()/1e6 + lifetime

public:
    // lifetimeMs: tiempo de vida en milisegundos (como pide el enunciado)
    Wasp(Texture* t, Point2D p, Vector2D<> v, Uint32 lifetimeMs);

    void render() const;
    void update();
    bool isAlive() const;
    Collision checkCollision(const SDL_FRect& rect) const;

    const Point2D& getPos() const { return pos; }
    const Vector2D<>& getVel() const { return vel; }
};
