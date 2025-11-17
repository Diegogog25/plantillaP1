#pragma once
#include "SceneObject.h"

class Wasp : public SceneObject {
    unsigned long long expireAtMs = 0;
public:
    Wasp(Game* g, Texture* t, float X, float Y, unsigned ms)
        : SceneObject(g, t, X, Y, (float)t->getFrameWidth(), (float)t->getFrameHeight())
    {
        expireAtMs = (unsigned long long)(SDL_GetTicksNS() / 1000000ULL) + ms;
    }

    void update() override;
    void render() const override { SceneObject::render(); }
    bool isAlive() const;

    Collision checkCollision(const SDL_FRect& other) const override {
        SDL_FRect me = bbox();
        if (SDL_HasRectIntersectionFloat(&me, &other))
            return { Collision::Type::ENEMY, {} };
        return {};
    }
};
