#pragma once
#include "vector2D.h"
#include "texture.h"
#include "SDL3/SDL.h"
#include "Collision.h"

class HomedFrog
{
private:
    Texture* texture = nullptr;
    Point2D  pos;

public:
    HomedFrog(Texture* t, Point2D p);
    void render() const;
    Collision checkCollision(const SDL_FRect& rect) const;

    const Point2D& getPos() const { return pos; }
};
