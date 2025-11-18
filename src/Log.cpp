#include "Log.h"
#include "game.h"
#include "Errors.h"
#include <istream>

Log::Log(Game* g, Texture* tex, // constructor
    Point2D pos,
    float vx,
    float leftSpan, float rightSpan)
    : Platform(g, tex,
        pos,
        (float)tex->getFrameWidth(),
        (float)tex->getFrameHeight(),
        Vector2D<>{vx, 0.0f},
        leftSpan, rightSpan)
{
    vel = { vx, 0 };
}

SceneObject* Log::FromMap(Game* g, std::istream& ss, const char* path, int lineNum) // lectura desde mapa de log
{
    float x, y, vx; int type;
    if (!(ss >> x >> y >> vx >> type))
        throw FileFormatError(path, lineNum, "Invalid Log line");

    Texture* t = (type == 1) ? g->getTexture(Game::LOG2) : g->getTexture(Game::LOG1);

    const float leftSpan = -150.0f;
    const float rightSpan = (float)Game::WINDOW_WIDTH + 150.0f;

    return new Log(g, t, Point2D(x, y), vx / Game::FRAME_RATE, leftSpan, rightSpan);
}

void Log::update() { // actualización de posición y límites
    pos = pos + vel;

    if (vel.getX() > 0 && pos.getX() > rightLimit)
        pos.setX(leftLimit);

    else if (vel.getX() < 0 && pos.getX() + w < leftLimit)
        pos.setX(rightLimit);
}
Collision Log::checkCollision(const SDL_FRect& other) { // colisiones con la rana, le aplica su velocidad

    SDL_FRect me{ pos.getX() , pos.getY(), w, h };

    if (SDL_HasRectIntersectionFloat(&me, &other))
            return { Collision::Type::PLATFORM, vel };
    return {};
}

Log::~Log() = default; // destructor

