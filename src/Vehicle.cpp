#include "Vehicle.h"

Vehicle::Vehicle(Texture* _texture, Point2D _pos, Vector2D<> _vel) {
    texture = _texture;
    pos = _pos;
    vel = _vel;
}
Vehicle::~Vehicle() {}

void Vehicle::render() const {
    SDL_FRect destRect{ pos.getX(), pos.getY(), (float)texture->getFrameWidth(), (float)texture->getFrameHeight() };
    texture->render(destRect);
}
void Vehicle::update() {
    Vector2D<> _v{ vel.getX(), vel.getY() };
    Vector2D<> _start{ -150.0f, pos.getY() };
    Vector2D<> _end{ 598.0f,  pos.getY() };
    if (pos.getX() >= 598.0f) pos = _start;
    else if (pos.getX() <= -150.0f) pos = _end;
    pos = pos + _v;
}

Collision Vehicle::checkCollision(const SDL_FRect& rect) const {
    SDL_FRect me{ pos.getX(), pos.getY(), (float)texture->getFrameWidth(), (float)texture->getFrameHeight() };
    if (SDL_HasRectIntersectionFloat(&rect, &me))
        return Collision{ Collision::Type::ENEMY, {} };
    return {};
}
