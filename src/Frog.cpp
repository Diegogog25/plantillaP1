#include "Frog.h"
#include "game.h"
#include <cmath>

void Frog::render() const {
    SDL_FRect r{ pos.getX(), pos.getY(), w, h};
    const int col = (moving || jumpFrames > 0) ? 1 : 0;
    SDL_FPoint center{ r.w * 0.5f, r.h * 0.5f };
    float a = std::fmod(angle, 360.0f); if (a < 0) a += 360.0f;
    tex->renderFrame(r, 0, col, a, &center, SDL_FLIP_NONE);
}

void Frog::update() {
    if (moving) {
		pos += step;
        if (--framesLeft <= 0) moving = false;
        if (jumpFrames > 0) --jumpFrames;
    }

    SDL_FRect box{ pos.getX(), pos.getY() + 8.0f, w, std::max(1.f, h - 16.f)};

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
             pos += c.platformVel;
             std::cout << "plataforma";
             if (pos.getX() < 0 || pos.getX() + w > Game::WINDOW_WIDTH) { loseLife(); reset(); return; }
        }
        else if (c.type == Collision::Type::NONE) {
            // agua
            if (pos.getY() < Game::RIVER_LOW) { loseLife(); reset(); return; }
        }
    }
}

void Frog::handleEvents(const SDL_Event& e) {
    if (e.type != SDL_EVENT_KEY_DOWN || e.key.repeat) return;
    if (moving || jumpFrames > 0) return;

    Point2D dir;
    switch (e.key.key) {
    case SDLK_W: case SDLK_UP:    dir.setY(-1); angle = 0.f; break;
    case SDLK_S: case SDLK_DOWN:  dir.setY(1); angle = 180.f; break;
    case SDLK_A: case SDLK_LEFT:  dir.setX(-1); angle = 270.f; break;
    case SDLK_D: case SDLK_RIGHT: dir.setX(1); angle = 90.f; break;
    default: return;
    }
    dir *= cell;
    Point2D limits = pos + dir;
    if (limits.getX() < 0 || limits.getX() + w > Game::WINDOW_WIDTH) return;
    if (limits.getY() < 0 || limits.getY() + h > Game::WINDOW_HEIGHT - uiH) return;

    step.setX(dir.getX() / (float)JUMP_FRAMES);
    step.setY(dir.getY() / (float)JUMP_FRAMES);
    framesLeft = JUMP_FRAMES;
    moving = true; jumpFrames = JUMP_FRAMES;
}
