#include "SceneObject.h"

SceneObject::SceneObject(Game* g, Texture* t, // constructor
    Point2D pos,
    float W, float H)
    : GameObject(g), pos(pos), w(W), h(H), tex(t)
{}

SceneObject::~SceneObject() = default; // destructor

SDL_FRect SceneObject::getBoundingBox() const { // getter de la caja de colisión
    return SDL_FRect{ pos.getX(), pos.getY(), w, h };
}

void SceneObject::update() {} // actualización (vacía)

void SceneObject::render() const { // renderizado
    if (tex) {
        SDL_FRect r{ pos.getX(), pos.getY(), w, h };
        tex->render(r);
    }
}

Collision SceneObject::checkCollision(const SDL_FRect& other) { // colisiones
    SDL_FRect me = getBoundingBox();
    Collision col;
    if (SDL_HasRectIntersectionFloat(&me, &other))
        col.type = Collision::Type::ENEMY;
    else col.type = Collision::Type::NONE;

    return col;
}