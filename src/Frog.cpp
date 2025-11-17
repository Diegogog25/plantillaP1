#include "Frog.h"
#include "game.h"
#include <cmath>

void Frog::render() const {
    SDL_FRect r{ x, y, w, h };
    const int col = (moving || jumpFrames > 0) ? 1 : 0;
    SDL_FPoint center{ r.w * 0.5f, r.h * 0.5f };
    float a = std::fmod(angle, 360.0f); if (a < 0) a += 360.0f;
    tex->renderFrame(r, 0, col, a, &center, SDL_FLIP_NONE);
}

void Frog::update() {
    if (moving) {
        x += stepX; y += stepY;
        if (--framesLeft <= 0) moving = false;
        if (jumpFrames > 0) --jumpFrames;
    }

    SDL_FRect box{ x, y + 8.f, w, std::max(1.f, h - 16.f) };

    // 1ª pasada: ENEMY/HOME
    Collision c = game->checkCollision(box);
    if (moving) {
        if (c.type == Collision::Type::ENEMY) { loseLife(); reset(); return; }
        if (c.type == Collision::Type::HOME) { reset(); return; } // Game marcará nido ocupado
        return;
    }
    else {
        if (c.type == Collision::Type::ENEMY) { loseLife(); reset(); return; }
        else if (c.type == Collision::Type::HOME) { reset(); return; } // Game marcará nido ocupado
        else if (c.type == Collision::Type::PLATFORM) {
             x += c.platformVel.getX();
             std::cout << "plataforma";
             if (x < 0 || x + w > Game::WINDOW_WIDTH) { loseLife(); reset(); return; }
        }
        else if (c.type == Collision::Type::NONE) {
            // agua
            if (y < Game::RIVER_LOW) { loseLife(); reset(); return; }
        }
    }
}

void Frog::handleEvents(const SDL_Event& e) {
    if (e.type != SDL_EVENT_KEY_DOWN || e.key.repeat) return;
    if (moving || jumpFrames > 0) return;

    int dx = 0, dy = 0;
    switch (e.key.key) {
    case SDLK_W: case SDLK_UP:    dy = -1; angle = 0.f; break;
    case SDLK_S: case SDLK_DOWN:  dy = 1; angle = 180.f; break;
    case SDLK_A: case SDLK_LEFT:  dx = -1; angle = 270.f; break;
    case SDLK_D: case SDLK_RIGHT: dx = 1; angle = 90.f; break;
    default: return;
    }

    float nx = x + dx * cell, ny = y + dy * cell;
    if (nx < 0 || nx + w > Game::WINDOW_WIDTH) return;
    if (ny < 0 || ny + h > Game::WINDOW_HEIGHT - uiH) return;

    stepX = (dx * cell) / (float)JUMP_FRAMES;
    stepY = (dy * cell) / (float)JUMP_FRAMES;
    framesLeft = JUMP_FRAMES;
    moving = true; jumpFrames = JUMP_FRAMES;
}
