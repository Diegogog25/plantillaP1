#include "Log.h"

Log::Log(Texture* _texture, Point2D _pos, Vector2D<> _vel) {
    texture = _texture;
    pos = _pos;
    vel = _vel;
}
Log::~Log() {}

void Log::render() const {
    SDL_FRect destRect{ pos.getX(), pos.getY(), (float)texture->getFrameWidth(), (float)texture->getFrameHeight() };
    texture->render(destRect);
}
void Log::update() {
    Vector2D<> _v{ vel.getX(), vel.getY() };
    Vector2D<> _start{ -150.0f, pos.getY() };
    Vector2D<> _end{ 598.0f,  pos.getY() };
    if (pos.getX() >= 598.0f) pos = _start;
    else if (pos.getX() <= -150.0f) pos = _end;
    pos = pos + _v;
}

Collision Log::checkCollision(const SDL_FRect& rect) const {
    SDL_FRect me{ pos.getX(), pos.getY(), (float)texture->getFrameWidth(), (float)texture->getFrameHeight() };
    if (SDL_HasRectIntersectionFloat(&rect, &me))
        return Collision{ Collision::Type::PLATFORM, vel };
    return {};
}
