#include "Log.h"

Log::Log(Game* g, Texture* tex,
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
}

Log::~Log() = default;

