#include "Wasp.h"
#include "game.h"
#include <SDL3/SDL_timer.h>

Wasp::Wasp(Game* g, Texture* t, const Point2D& pos, Uint32 lifetimeMs)
    : SceneObject(g, t, pos, (float)t->getFrameWidth(), (float)t->getFrameHeight())
{
    expireAtMs = (unsigned long long)(SDL_GetTicksNS() / 1000000ULL) + lifetimeMs;
}

void Wasp::update() {
    expireAtMs--;
}

Wasp::~Wasp(){}

bool Wasp::isAlive() const {
    unsigned long long now = (unsigned long long)(SDL_GetTicksNS() / 1000000ULL);
    return now < expireAtMs;
}

Collision Wasp::checkCollision(const SDL_FRect& other) {
    SDL_FRect me = bbox();
    if (SDL_HasRectIntersectionFloat(&me, &other))
        return { Collision::Type::ENEMY, {} };
    return {};
}
