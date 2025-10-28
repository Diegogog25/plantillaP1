#pragma once
#include "texture.h"
#include "vector2D.h"
#include "SDL3/SDL.h"
#include "collision.h"

class HomedFrog
{
private:
    Texture* texture = nullptr;
    Point2D  pos;
    bool inHouse = false;

public:
    HomedFrog(Texture* t, Point2D p);
    void render() const;
    Collision checkCollision(const SDL_FRect& rect);
    const Point2D& getPos() const { return pos; }
    bool house() const { return inHouse;}
};
