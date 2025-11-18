#include "Vehicle.h"
#include "game.h"
#include "Errors.h"
#include <istream>

Vehicle::Vehicle(Game* g, Texture* tex, // constructor
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

Vehicle::~Vehicle() = default; // destructor

SceneObject* Vehicle::FromMap(Game* g, std::istream& ss, const char* path, int lineNum) // lectura desde mapa de vehículos
{
    float x, y, vx; int type;
    if (!(ss >> x >> y >> vx >> type))
        throw FileFormatError(path, lineNum, "Invalid Vehicle line");

    Texture* t =
        (type == 1) ? g->getTexture(Game::CAR1) :
        (type == 2) ? g->getTexture(Game::CAR2) :
        (type == 3) ? g->getTexture(Game::CAR3) :
        (type == 4) ? g->getTexture(Game::CAR4) :
        g->getTexture(Game::CAR5);

    // Usa el ancho real del sprite para calcular los spans y reducir el desfase
    const float w = (float)t->getFrameWidth();
    const float leftSpan = -w;
    const float rightSpan = (float)Game::WINDOW_WIDTH + w;

    return new Vehicle(g, t, Point2D(x, y), vx / Game::FRAME_RATE, leftSpan, rightSpan);
}

void Vehicle::render() const { // renderizado
    SceneObject::render();
}

Collision Vehicle::checkCollision(const SDL_FRect& other) { // colisiones
    SDL_FRect me = getBoundingBox();
    if (SDL_HasRectIntersectionFloat(&me, &other))
        return { Collision::Type::ENEMY, {} };
    return {};
}
