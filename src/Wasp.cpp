#include "Wasp.h"

Wasp::Wasp(Texture* t, Point2D p, Uint32 lifetimeMs)
    : texture(t), pos(p)
{
    // Guardamos el instante de caducidad en milisegundos
    const unsigned long long nowMs = (unsigned long long)(SDL_GetTicksNS() / 1000000ULL);
    expireAtMs = nowMs + (unsigned long long)lifetimeMs;
}

void Wasp::render() const {
    SDL_FRect r{ pos.getX(), pos.getY(),
                 (float)texture->getFrameWidth(),
                 (float)texture->getFrameHeight() };
    texture->render(r);
}

void Wasp::update() {
}

bool Wasp::isAlive() const {
    const unsigned long long nowMs = (unsigned long long)(SDL_GetTicksNS() / 1000000ULL);
    return nowMs < expireAtMs;
}

Collision Wasp::checkCollision(const SDL_FRect& rect) const {
    SDL_FRect me{ pos.getX(), pos.getY(),
                  (float)texture->getFrameWidth(),
                  (float)texture->getFrameHeight() };
    if (SDL_HasRectIntersectionFloat(&rect, &me)) {
        return Collision{ Collision::Type::ENEMY, {} };
    }
    return {};
}
