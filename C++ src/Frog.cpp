#include "Frog.h"
#include "game.h"
#include "Errors.h"
#include <cmath>
#include <istream>
#include <sstream>

Frog* Frog::FromMap(Game* g, std::istream& ss, const char* path, int lineNum)
{
    float x, y;
    if (!(ss >> x >> y))
        throw FileFormatError(path, lineNum, "Invalid Frog line");
    return new Frog(g, g->getTexture(Game::FROG), Point2D(x, y));
}

void Frog::render() const {
    SDL_FRect r{ pos.getX(), pos.getY(), w, h};
    const int col = (moving || jumpFrames > 0) ? 1 : 0;
    SDL_FPoint center{ r.w * 0.5f, r.h * 0.5f };
    float a = std::fmod(angle, 360.0f); if (a < 0) a += 360.0f;
    tex->renderFrame(r, 0, col, a, &center, SDL_FLIP_NONE);
}

void Frog::update() {
    // 1) Avance del paso suave a 30 FPS
    if (moving) {
        pos = pos + stepPerFrame;
        if (--moveFramesLeft <= 0) {
            pos = moveEnd;          // asegura acabar exacto en el destino
            moving = false;
            // opcional: dejar unos frames en salto
        } else {
            if (jumpFrames < JUMP_FRAMES) ++jumpFrames;
        }
    } else if (jumpFrames > 0) {
        --jumpFrames;
    }

    // 2) Límites de ventana
    const float w = (float)texture->getFrameWidth();
    const float h = (float)texture->getFrameHeight();
    pos.setX(std::clamp(pos.getX(), 0.0f, (float)Game::WINDOW_WIDTH  - w));
    pos.setY(std::clamp(pos.getY(), 0.0f, (float)Game::WINDOW_HEIGHT - h));

    // 3) Colisiones (AABB con margen vertical global)
    const float vMargin = 8.0f;
    const float ch = std::max(1.0f, h - 2.0f * vMargin);
    SDL_FRect collider{ pos.getX(), pos.getY() + vMargin, w, ch };

    Collision c = game->checkCollision(collider);

    if (moving) {
        // Arreglo principal: NO aplicar arrastre de plataforma durante el salto,
        // solo permitir que enemigos te maten en el aire.
        if (c.type == Collision::Type::ENEMY) {
            loseLife();
            resetToStart();
            return;
        }
        // Ignorar PLATFORM y agua mientras dura el movimiento suave.
    } else {
        switch (c.type) {
        case Collision::Type::ENEMY:
            loseLife();
            resetToStart();
            return;
        case Collision::Type::PLATFORM:
            // Arrastre horizontal del tronco (por frame) cuando ya has aterrizado
            pos = pos + Vector2D<>{ c.platformVel.getX(), 0.0f };
            pos.setX(std::clamp(pos.getX(), 0.0f, (float)Game::WINDOW_WIDTH - w));
            break;
        case Collision::Type::NONE:
        default:
            // Ahogarse solo cuando estás parado (no en mitad del salto)
            if (pos.getY() < Game::RIVER_LOW) {
                loseLife();
                resetToStart();
                return;
            }
            break;
        }
    }
}

void Frog::handleEvents(const SDL_Event& e) {
    if (e.type == SDL_EVENT_KEY_DOWN && !e.key.repeat) {
        // No aceptar nuevas teclas mientras:
        // - hay un movimiento en curso, o
        // - la animación anterior no ha terminado todavía.
        if (moving || jumpFrames > 0) return;

        int dx = 0, dy = 0;
        switch (e.key.key) {
        case SDLK_UP:    case SDLK_W: dy = -1; angle = 0.0f;   break;
        case SDLK_DOWN:  case SDLK_S: dy =  1; angle = 180.0f; break;
        case SDLK_LEFT:  case SDLK_A: dx = -1; angle = 270.0f; break;
        case SDLK_RIGHT: case SDLK_D: dx =  1; angle = 90.0f;  break;
        default: break;
        }

        // Inicia el movimiento por tiempo sin forzar pos final
        stepPerFrame = Vector2D<>{ (dx * speed) / (float)JUMP_FRAMES,
                                   (dy * speed) / (float)JUMP_FRAMES };
        moveFramesLeft = (dx != 0 || dy != 0) ? JUMP_FRAMES : 0;
        moving = (dx != 0 || dy != 0);

        if (moving) {
            // Solo arrancar la animación cuando realmente empieza un nuevo movimiento
            jumpFrames = JUMP_FRAMES;
        }
    }
}