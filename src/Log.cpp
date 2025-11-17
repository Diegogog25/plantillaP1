#include "Log.h"

Log::Log(Game* g, Texture* tex,
    float x, float y,
    float vx,
    float leftSpan, float rightSpan)
    : Platform(g, tex,
        x, y,
        (float)tex->getFrameWidth(),
        (float)tex->getFrameHeight(),
        Vector2D<>{vx, 0.0f},
        leftSpan, rightSpan)
{
}

Log::~Log() = default;

