#include "HomedFrog.h"

HomedFrog::HomedFrog(Texture* t, Point2D p) : texture(t), pos(p) {}

void HomedFrog::render() const {
    //
    if (!inHouse)return;

    SDL_FRect r{ pos.getX(), pos.getY(),
                 (float)texture->getFrameWidth(),
                 (float)texture->getFrameHeight() };
    texture->renderFrame(r, 0, 0 , 180, nullptr, SDL_FLIP_NONE);
}

Collision HomedFrog::checkCollision(const SDL_FRect& rect) {
    Collision h;
    SDL_FRect me{ pos.getX(), pos.getY(),
                  (float)texture->getFrameWidth(),
                  (float)texture->getFrameHeight() };
    if (SDL_HasRectIntersectionFloat(&rect, &me)) {
     //Comprueba si esta en casa o no, para hacer visible la homed frog o no
        if (inHouse) {
            h = Collision{ Collision::Type::ENEMY, {} };
        }
        else {
            h = Collision{ Collision::Type::HOME, {} };
            inHouse = true;
        }       
    }
    return h;
}
