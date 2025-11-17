#include "Wasp.h"

void Wasp::update() { /* sin movimiento propio */ }

bool Wasp::isAlive() const {
    unsigned long long now = (unsigned long long)(SDL_GetTicksNS() / 1000000ULL);
    return now < expireAtMs;
}
