#include "Log.h"
#include "game.h"

Log::Log(Game* g, Texture* tex,
    Point2D pos,
    float vx,
    float leftSpan, float rightSpan)
    : Platform(g, tex,
        pos,
        (float)tex->getFrameWidth(),
        (float)tex->getFrameHeight(),
        Vector2D<>{vx, 0.0f},
        leftSpan, rightSpan)
{
    vel = { vx, 0 };
}

void Log::update() {
    pos = pos + vel;

    if (vel.getX() > 0 && pos.getX() > rightLimit)
        pos.setX(leftLimit);

    else if (vel.getX() < 0 && pos.getX() + w < leftLimit)
        pos.setX(rightLimit);
}
Collision Log::checkCollision(const SDL_FRect& other)  {

    SDL_FRect me{ pos.getX() , pos.getY(), w, h };

    if (SDL_HasRectIntersectionFloat(&me, &other))
            return { Collision::Type::PLATFORM, vel };
    return {};
}

Log::~Log() = default;

