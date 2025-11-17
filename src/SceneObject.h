#pragma once
#include "GameObject.h"
#include "texture.h"
#include "collision.h"
#include <SDL3/SDL.h>

class SceneObject : public GameObject {
protected:
    float x{}, y{}, w{}, h{};
    Texture* tex{};

    SDL_FRect bbox() const { return SDL_FRect{ x, y, w, h }; }

public:
    SceneObject(Game* g, Texture* t, float X, float Y, float W, float H)
        : GameObject(g), x(X), y(Y), w(W), h(H), tex(t) {
    }

    virtual ~SceneObject() = default;

    // Ya NO es abstracta: update vacío
    void update() override {}

    void render() const override {
        if (tex)
            tex->render(SDL_FRect{ x, y, w, h });
    }

    // Getter público para el AABB (para usar desde Game)
    SDL_FRect getRect() const { return bbox(); }

    // Por defecto, colisión ENEMY si se solapan
    virtual Collision checkCollision(const SDL_FRect& other) const {
        SDL_FRect me = bbox();
        if (SDL_HasRectIntersectionFloat(&me, &other))
            return { Collision::Type::ENEMY, {} };
        return {};
    }
};
