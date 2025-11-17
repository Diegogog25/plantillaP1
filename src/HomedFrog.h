#pragma once
#include "SceneObject.h"

class HomedFrog : public SceneObject {
    bool inHouse = false;
public:
    HomedFrog(Game* g, Texture* t, float X, float Y)
        : SceneObject(g, t, X, Y,
            (float)t->getFrameWidth(),
            (float)t->getFrameHeight()) {
    }

    ~HomedFrog() override = default;

    // ? implementación vacía, ya no es abstracta
    void update() override {}

    void setOccupied(bool v) { inHouse = v; }
    bool isOccupied() const { return inHouse; }

    void render() const override {
        if (!inHouse) return;        // o quita esto si quieres ver siempre la casa
        SceneObject::render();
    }

    Collision checkCollision(const SDL_FRect& rect) const override {
        SDL_FRect me = getRect();
        if (!SDL_HasRectIntersectionFloat(&me, &rect))
            return {};

        // Si ya está ocupada, se considera ENEMY; si no, HOME
        return inHouse
            ? Collision{ Collision::Type::ENEMY, {} }
        : Collision{ Collision::Type::HOME,  {} };
    }
};
