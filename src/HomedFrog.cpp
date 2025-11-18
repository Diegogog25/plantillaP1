#include "HomedFrog.h"
#include "game.h"


HomedFrog::HomedFrog(Game* g, Texture* t, Point2D pos) // constructor
    : SceneObject(g, t, pos,
        (float)t->getFrameWidth(),
        (float)t->getFrameHeight()) {
    inHouse = false;
}

HomedFrog::~HomedFrog() = default; // destructor

void HomedFrog::update() {} // actualización (vacía)

void HomedFrog::setOccupied(bool v) { // marcar como ocupado o libre
    inHouse = v;
}

bool HomedFrog::isOccupied() const { // consultar si está ocupado
    return inHouse;
}

void HomedFrog::render() const { // renderizado
    if (inHouse)
    {
        SDL_FRect destRect{ pos.getX(), pos.getY(), (float)tex->getFrameWidth(), (float)tex->getFrameHeight() };
        tex->renderFrame(destRect, 0, 0, 180, nullptr, SDL_FLIP_NONE);
    }
}

Collision HomedFrog::checkCollision(const SDL_FRect& rect) { // colisiones, cambia según si está ocupado o no
    // Banda superior de la casa: reduce la altura de colisión
    constexpr float HOME_BAND = 0.30f; // 30% superior
    const float bandH = std::max(1.f, h * HOME_BAND);
    SDL_FRect me{ pos.getX(), pos.getY(), w, bandH };

    if (!SDL_HasRectIntersectionFloat(&me, &rect))
        return {};

    if (inHouse) {
        return { Collision::Type::ENEMY, {} };
    } else {
        inHouse = true; // se ocupa al entrar
        return { Collision::Type::HOME, {} };
    }
}
