#include "Vehicle.h"
#include "SceneObject.h"

Vehicle::Vehicle(Game* g, Texture* tex,
    Point2D pos,
    float vx,
    float leftSpan, float rightSpan)
    : Crosser(g, tex,
        pos,
        (float)tex->getFrameWidth(),
        (float)tex->getFrameHeight(),
        Vector2D<>{vx, 0.0f},
        leftSpan, rightSpan)
{}

Vehicle::~Vehicle() = default;

void Vehicle::render() const {
    SceneObject::render();
}

Collision Vehicle::checkCollision(const SDL_FRect& other) const {
    SDL_FRect me = getRect();
    if (SDL_HasRectIntersectionFloat(&me, &other))
        return { Collision::Type::ENEMY, {} };
    return {};
}
