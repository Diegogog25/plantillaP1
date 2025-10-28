#include "Log.h"
#include "game.h"
#include <algorithm>

Log::Log(Texture* _texture, Point2D _pos, Vector2D<> _vel) {
    texture = _texture;
    pos = _pos;
    vel = _vel;
}

Log::~Log() {}

void Log::render() const {
    SDL_FRect destRect{
        pos.getX(),
        pos.getY(),
        (float)texture->getFrameWidth(),
        (float)texture->getFrameHeight()
    };
    texture->renderFrame(destRect, 0, 0);
}

void Log::update() {
    const float fw = (float)texture->getFrameWidth();
    const float windowW = (float)Game::WINDOW_WIDTH;

    // Mover primero
    pos = pos + vel;

    // Envolver manteniendo desfase (usa while por si el salto es grande)
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

Collision Log::checkCollision(const SDL_FRect& rect) const {
    // Rect del tronco basado en el sprite
    SDL_FRect me{
        pos.getX(),
        pos.getY(),
        (float)texture->getFrameWidth(),
        (float)texture->getFrameHeight()
    };

    // Recorta ligeramente el AABB del tronco para evitar que la rana
    // "enganche" por los bordes cuando está al lado y no encima.
    const float insetX = me.w * 0.15f;      // ~15% por cada lado
    const float insetTop = me.h * 0.10f;    // ~10% arriba
    const float insetBottom = me.h * 0.10f; // ~10% abajo

    me.x += insetX;
    me.w = std::max(1.0f, me.w - 2.0f * insetX);
    me.y += insetTop;
    me.h = std::max(1.0f, me.h - insetTop - insetBottom);

    if (SDL_HasRectIntersectionFloat(&rect, &me))
        return Collision{ Collision::Type::PLATFORM, vel };

    return {};
}
