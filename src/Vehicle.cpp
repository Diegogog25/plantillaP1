#include "Vehicle.h"
#include "game.h"

Vehicle::Vehicle(Texture* _texture, Point2D _pos, Vector2D<> _vel) {
    texture = _texture;
    pos = _pos;
    vel = _vel;
}
Vehicle::~Vehicle() {}

void Vehicle::render() const {
    SDL_FRect destRect{
        pos.getX(),
        pos.getY(),
        (float)texture->getFrameWidth(),
        (float)texture->getFrameHeight()
    };
    texture->renderFrame(destRect, 0, 0);
}

void Vehicle::update() {
    const float fw = (float)texture->getFrameWidth();
    const float windowW = (float)Game::WINDOW_WIDTH;

    // mover primero
    pos = pos + vel;

    // envolver manteniendo desfase
    if (vel.getX() > 0.0f) {
        while (pos.getX() > windowW) {
            pos.setX(pos.getX() - (windowW + fw));
        }
    } else if (vel.getX() < 0.0f) {
        while (pos.getX() + fw < 0.0f) {
            pos.setX(pos.getX() + (windowW + fw));
        }
    }
}

Collision Vehicle::checkCollision(const SDL_FRect& rect) const {
    SDL_FRect me{ pos.getX(), pos.getY(), (float)texture->getFrameWidth(), (float)texture->getFrameHeight() };
    if (SDL_HasRectIntersectionFloat(&rect, &me))
        return Collision{ Collision::Type::ENEMY, {} };
    return {};
}
