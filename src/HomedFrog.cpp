#include "HomedFrog.h"

HomedFrog::HomedFrog(Game* g, Texture* t, Point2D pos)
    : SceneObject(g, t, pos,
        (float)t->getFrameWidth(),
        (float)t->getFrameHeight()) {
}

HomedFrog::~HomedFrog() = default;

void HomedFrog::update() {}

void HomedFrog::setOccupied(bool v) {
    inHouse = v;
}

bool HomedFrog::isOccupied() const {
    return inHouse;
}

void HomedFrog::render() const {
    if (!inHouse) return; // solo renderiza si la casa está ocupada
    SceneObject::render();
}

Collision HomedFrog::checkCollision(const SDL_FRect& rect) const {
    SDL_FRect me = getRect();
    if (!SDL_HasRectIntersectionFloat(&me, &rect))
        return {};

    // Si ya está ocupada, se considera ENEMY; si no, HOME
    return inHouse
        ? Collision{ Collision::Type::ENEMY, {} }
    : Collision{ Collision::Type::HOME,  {} };
}
