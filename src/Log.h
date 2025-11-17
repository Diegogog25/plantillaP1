// Log.h
#pragma once
#include "Platform.h"

class Log : public Platform {
public:
    // Heredar el constructor de Platform:
    // Log(Game* g, Texture* t, float x, float y, float w, float h,
    //     float vx, float vy, float span)
    using Platform::Platform;

    // Si quisieras un comportamiento especial,
    // podrías sobreescribir render() o checkCollision() aquí.
    // Pero no es obligatorio.
};

