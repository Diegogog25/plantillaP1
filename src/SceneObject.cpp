#include "SceneObject.h"


SceneObject::SceneObject(Game* g, Texture* t,
    float X, float Y,
    float W, float H)
    : GameObject(g), x(X), y(Y), w(W), h(H), tex(t)
{}

SceneObject::~SceneObject() = default;

SDL_FRect SceneObject::bbox() const {
    return SDL_FRect{ x, y, w, h };
}

void SceneObject::update() {}

void SceneObject::render() const {
    if (tex)
        tex->render(SDL_FRect{ x, y, w, h });
}

SDL_FRect SceneObject::getRect() const {
    return bbox();
}

Collision SceneObject::checkCollision(const SDL_FRect& other) const {
    SDL_FRect me = bbox();
    if (SDL_HasRectIntersectionFloat(&me, &other))
        return { Collision::Type::ENEMY, {} };

    return {};
}