#pragma once
#include "Platform.h"

// Grupo de tortugas: n tortugas seguidas del tamaño de un sprite.
// Si submerging=true, se hunden cíclicamente (no se dibujan ni colisionan cuando están hundidas).
class TurtleGroup : public Platform {
    int  numTurtles;
    bool submerging;
    int  timer = 0;
    int  period = 180; // 6s a 30 FPS
    bool sunk = false;

public:
    TurtleGroup(Texture* t, Point2D p, Vector2D<> v,
        int n, bool sink)
        : Platform(t, p, v)
        , numTurtles(n)
        , submerging(sink)
    {
    }

    void update() override {
        Platform::update();   // mueve y envuelve como un tronco normal

        if (submerging) {
            timer = (timer + 1) % period;
            // 1/3 del tiempo hundidas
            sunk = (timer > (period * 2) / 3);
        }
    }

    void render() const override {
        if (submerging && sunk) return; // hundidas: invisibles

        const float fw = (float)texture->getFrameWidth();
        const float fh = (float)texture->getFrameHeight();

        for (int i = 0; i < numTurtles; ++i) {
            SDL_FRect dst{ pos.getX() + i * fw, pos.getY(), fw, fh };
            texture->renderFrame(dst, 0, 0);
        }
    }

    Collision checkCollision(const SDL_FRect& other) const override {
        if (submerging && sunk) return {}; // hundidas: no colisionan

        const float fw = (float)texture->getFrameWidth();
        const float fh = (float)texture->getFrameHeight();

        for (int i = 0; i < numTurtles; ++i) {
            SDL_FRect me{ pos.getX() + i * fw, pos.getY(), fw, fh };
            if (SDL_HasRectIntersectionFloat(&me, &other))
                // Usamos vel heredado de Platform
                return Collision{ Collision::Type::PLATFORM, vel };
        }
        return {};
    }
};
