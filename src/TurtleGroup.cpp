#include "TurtleGroup.h"
#include "game.h"
#include "Errors.h"
#include <istream>
#include <algorithm>

TurtleGroup::TurtleGroup(Game* g, Texture* tex, // constructor
    Point2D pos,
    float w, float h,
    float vx,
    float leftSpan, float rightSpan,
    int n, bool sink)
    : Platform(g, tex, pos, w, h, { vx, 0.0f }, leftSpan, rightSpan)
    , numTurtles(n)
    , submerging(sink)
{
}

SceneObject* TurtleGroup::FromMap(Game* g, std::istream& ss, const char* path, int lineNum) // lectura desde mapa de las tortugas
{
    float x, y, vx; int n, sink;
    if (!(ss >> x >> y >> vx >> n >> sink))
        throw FileFormatError(path, lineNum, "Invalid TurtleGroup line");

    Texture* tex = g->getTexture(Game::TURTLE);
    float w = (float)tex->getFrameWidth();
    float h = (float)tex->getFrameHeight();

    // Ancho total del grupo
    float groupW = w * n;

    float leftSpan = -groupW;
    float rightSpan = (float)Game::WINDOW_WIDTH + groupW;

    return new TurtleGroup(g, tex, Point2D(x, y), w, h, vx / Game::FRAME_RATE,
        leftSpan, rightSpan, n, sink != 0);
}

static int AnimacionTortugas(int t, int period, bool submerging, bool& visible, bool& solid) // animación de las tortugas por fases, le devuelve el frame actual al render
{
    constexpr int SWIM_FRAMES = 3;
    constexpr int DIVE_FRAMES = 4;

    if (!submerging) {
        visible = true;
        solid = true;
        int slice = std::max(1, period / SWIM_FRAMES);
        int step = (t / slice) % SWIM_FRAMES;
        return step;
    }

    const int swimSpan = period * 50 / 100;
    const int diveDownSpan = period * 10 / 100;
    const int hideSpan = period * 30 / 100;
    const int diveUpSpan = period * 10 / 100;

    if (t < swimSpan) {
        visible = true; solid = true;
        int step = (t * SWIM_FRAMES) / std::max(1, swimSpan);
        return std::min(SWIM_FRAMES - 1, step);
    }
    t -= swimSpan;

    if (t < diveDownSpan) {
        visible = true;
        solid = t < (diveDownSpan * 2) / 3;
        int step = (t * DIVE_FRAMES) / std::max(1, diveDownSpan);
        return 3 + std::min(DIVE_FRAMES - 1, step);
    }
    t -= diveDownSpan;

    if (t < hideSpan) {
        visible = false; solid = false;
        return 6;
    }
    t -= hideSpan;

    if (t < diveUpSpan) {
        visible = true;
        solid = t > (diveUpSpan * 1) / 3;
        int step = (t * DIVE_FRAMES) / std::max(1, diveUpSpan);
        int frame = 6 - std::min(DIVE_FRAMES - 1, step);
        return frame;
    }

    visible = true; solid = true;
    return 0;
}

void TurtleGroup::update() { // actualización de la posición y animación
    Platform::update();
    timer = (timer + 1) % period;

    bool vis, solid;
    int animTime = timer;
    (void)AnimacionTortugas(animTime, period, submerging, vis, solid);
    sunk = !solid;
}

void TurtleGroup::render() const { // renderizado del frame actual
    bool visible, solid;
    int animTime = timer; 
    int col = AnimacionTortugas(animTime, period, submerging, visible, solid);
    if (!visible) return;

    for (int i = 0; i < numTurtles; ++i) {
        SDL_FRect dst{ pos.getX() + i * w, pos.getY(), w, h };
        tex->renderFrame(dst, 0, col);
    }
}

Collision TurtleGroup::checkCollision(const SDL_FRect& other) { // colisiones según el frame actual, cuando se hunden no colisionan
    bool visible, solid;
    int animTime = timer;
    (void)AnimacionTortugas(animTime, period, submerging, visible, solid);
    if (!solid) return {};

    // Reducimos la caja de cada tortuga
    constexpr float PAD_X = 4.f;
    const float rw = std::max(1.0f, w - 2.0f * PAD_X);

    for (int i = 0; i < numTurtles; ++i) {
        SDL_FRect me{ pos.getX() + i * w + PAD_X, pos.getY(), rw, h };
        if (SDL_HasRectIntersectionFloat(&me, &other))
            return { Collision::Type::PLATFORM, vel };
    }
    return {};
}