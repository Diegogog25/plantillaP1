#include "HomedFrog.h"
#include "game.h"


HomedFrog::HomedFrog(Game* g, Texture* t, Point2D pos)
    : SceneObject(g, t, pos,
        (float)t->getFrameWidth(),
        (float)t->getFrameHeight()) {
    inHouse = false;
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
    if (inHouse)
    {
        SDL_FRect destRect{ pos.getX(), pos.getY(), tex->getFrameWidth(), tex->getFrameHeight() };
        tex->renderFrame(destRect, 0, 0, 180, nullptr, SDL_FLIP_NONE);
    }
}

Collision HomedFrog::checkCollision(const SDL_FRect& rect) {
    SDL_FRect me = getRect();
    Collision col;
    if (!SDL_HasRectIntersectionFloat(&me, &rect))
        return {};

    // Si ya está ocupada, se considera ENEMY; si no, HOME
    if (inHouse) col.type = Collision::Type::ENEMY;
    else 
    {
        col.type = Collision::Type::HOME;
        inHouse = true;
    } 
}
