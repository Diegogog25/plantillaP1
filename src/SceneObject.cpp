#include "SceneObject.h"

SceneObject::SceneObject(Game* g, Texture* t,
    Point2D pos,
    float W, float H)
    : GameObject(g), pos(pos), w(W), h(H), tex(t)
{}

SceneObject::~SceneObject() = default;

SDL_FRect SceneObject::bbox() const {
    return SDL_FRect{ pos.getX(), pos.getY(), w, h };
}

void SceneObject::update() {}

void SceneObject::render() const {
    if (tex) {
        SDL_FRect r{ pos.getX(), pos.getY(), w, h };
        tex->render(r);
    }
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