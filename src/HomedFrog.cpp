#include "HomedFrog.h"

HomedFrog::HomedFrog(Texture* t, Point2D p)
    : texture(t), pos(p) {
}

void HomedFrog::render() const {
    SDL_FRect r{ pos.getX(), pos.getY(),
                 (float)texture->getFrameWidth(),
                 (float)texture->getFrameHeight() };
    texture->render(r);
}

Collision HomedFrog::checkCollision(const SDL_FRect& rect) const {
    SDL_FRect me{ pos.getX(), pos.getY(),
                  (float)texture->getFrameWidth(),
                  (float)texture->getFrameHeight() };
    if (SDL_HasRectIntersectionFloat(&rect, &me)) {
        return Collision{ Collision::Type::ENEMY, {} };
    }
    return {};
}
