#include "Wasp.h"
#include "game.h"
#include <SDL3/SDL_timer.h>
#include "Errors.h"
#include <istream>

Wasp::Wasp(Game* g, Texture* t, const Point2D& pos, Uint32 lifetimeMs) // constructor
    : SceneObject(g, t, pos, (float)t->getFrameWidth(), (float)t->getFrameHeight())
{
	expireAtMs = (unsigned long long)(SDL_GetTicksNS() / 1000000ULL) + lifetimeMs; // tiempo de expiración
}

void Wasp::render() const { // renderizado
    SDL_FRect r{ pos.getX(), pos.getY(), w, h };
    tex->renderFrame(r, 0, 0, 180.0, nullptr, SDL_FLIP_NONE);
}

void Wasp::deleteWasp() { // marca la avispa para borrado
    game->deleteAfter(anchor);
}

void Wasp::update() { // actualización
    // Auto-eliminación cuando muera
    if (!isAlive()) {
        deleteWasp();
        return;
    }
}

Wasp::~Wasp() {} // destructor

bool Wasp::isAlive() const { // comprueba si la avispa sigue viva
    unsigned long long now = (unsigned long long)(SDL_GetTicksNS() / 1000000ULL);
    return now < expireAtMs;
}

Collision Wasp::checkCollision(const SDL_FRect& other) { // colisiones
    SDL_FRect me = getBoundingBox();
    if (SDL_HasRectIntersectionFloat(&me, &other))
        return { Collision::Type::ENEMY, {} };
    return {};
}

Wasp* Wasp::FromMap(Game* g, std::istream& ss, const char* path, int lineNum)
{
    float x, y;
    unsigned int lifetimeMs = 2500;
    if (!(ss >> x >> y))
        throw FileFormatError(path, lineNum, "Invalid Wasp line (faltan x y)");
    if (!(ss >> lifetimeMs)) {
        lifetimeMs = 2500; // lifetime opcional
    }
    Texture* t = g->getTexture(Game::WASP);
    return new Wasp(g, t, Point2D(x, y), lifetimeMs);
}