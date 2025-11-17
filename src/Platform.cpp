#include "Platform.h"
#include "game.h"
#include <algorithm>

Platform::Platform(Texture* tex, Point2D p, Vector2D<> v)
    : texture(tex), pos(p), vel(v)
{
}

Platform::~Platform() {}

void Platform::render() const {
    SDL_FRect destRect{
        pos.getX(),
        pos.getY(),
        (float)texture->getFrameWidth(),
        (float)texture->getFrameHeight()
    };
    texture->renderFrame(destRect, 0, 0);
}

void Platform::update() {
    const float fw = (float)texture->getFrameWidth();
    const float windowW = (float)Game::WINDOW_WIDTH;

    // Mover primero
    pos = pos + vel;

    // Envolver manteniendo desfase (igual que en Log)
    if (vel.getX() > 0.0f) {
        while (pos.getX() > windowW) {
            pos.setX(pos.getX() - (windowW + fw));
        }
    }
    else if (vel.getX() < 0.0f) {
        while (pos.getX() + fw < 0.0f) {
            pos.setX(pos.getX() + (windowW + fw));
        }
    }
}

Collision Platform::checkCollision(const SDL_FRect& rect) const {
    // Rect de la plataforma basado en el sprite
    SDL_FRect me{
        pos.getX(),
        pos.getY(),
        (float)texture->getFrameWidth(),
        (float)texture->getFrameHeight()
    };

    // Recorte suave del AABB para evitar “engancharse” en bordes
    const float insetX = me.w * 0.15f;
    const float insetTop = me.h * 0.10f;
    const float insetBottom = me.h * 0.10f;

    me.x += insetX;
    me.w = std::max(1.0f, me.w - 2.0f * insetX);
    me.y += insetTop;
    me.h = std::max(1.0f, me.h - insetTop - insetBottom);

    if (SDL_HasRectIntersectionFloat(&rect, &me))
        return Collision{ Collision::Type::PLATFORM, vel };

    return {};
}
