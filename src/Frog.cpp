#include "Frog.h"
#include "game.h"
#include "collision.h"
#include <SDL3/SDL.h>
#include <algorithm>
#include <cmath>

Frog::Frog(Point2D _pos, Game* _game) {
	Game::TextureName tname = Game::TextureName::FROG;
	game = _game;
    texture = game->getTexture(tname),
    pos = _pos,
    startPos = _pos,
    game = _game;
}
    

Frog::~Frog() {}

void Frog::render() const {
    SDL_FRect destRect{
        pos.getX(),
        pos.getY(),
        (float)texture->getFrameWidth(),
        (float)texture->getFrameHeight()
    };

    // 0 = quieta, 1 = salto/movimiento
    const int col = (moving || jumpFrames > 0) ? 1 : 0;

    // Centro RELATIVO al rect destino (SDL3)
    SDL_FPoint center{ destRect.w * 0.5f, destRect.h * 0.5f };

    float a = std::fmod(angle, 360.0f);
    if (a < 0.0f) a += 360.0f;

    texture->renderFrame(destRect, 0, col, a, &center, SDL_FLIP_NONE);
}

void Frog::update() {
    // 1) Avance del paso suave a 30 FPS
    if (moving) {
        pos = pos + stepPerFrame;
        if (--moveFramesLeft <= 0) {
            moving = false; // no forzar moveEnd
        }
    }

    // Animación acoplada al movimiento
    if (jumpFrames > 0) --jumpFrames;

    // 2) Límites de ventana (solo para movimiento propio; el arrastre del tronco se gestiona abajo)
    const float w = (float)texture->getFrameWidth();
    const float h = (float)texture->getFrameHeight();

    // 3) Colisiones (AABB con margen vertical global)
    const float vMargin = 8.0f;
    const float ch = std::max(1.0f, h - 2.0f * vMargin);
    SDL_FRect collider{ pos.getX(), pos.getY() + vMargin, w, ch };

    Collision c = game->checkCollision(collider);

    if (moving) {
        // En el aire te pueden matar enemigos o llegar a casa
        if (c.type == Collision::Type::ENEMY) {
            loseLife();
            resetToStart();
            return;
        }
        else if (c.type == Collision::Type::HOME){
                resetToStart();
                return; 
        }
        return; // ignorar plataforma/agua durante el salto
    }

    switch (c.type) {
    case Collision::Type::ENEMY:
        loseLife();
        resetToStart();
        return;

    case Collision::Type::PLATFORM: {
        // Arrastre horizontal del tronco
        pos = pos + Vector2D<>{ c.platformVel.getX(), 0.0f };

        // Si el arrastre te saca de la pantalla, muere y reinicia
        if (pos.getX() < 0.0f || (pos.getX() + w) > (float)Game::WINDOW_WIDTH) {
            loseLife();
            resetToStart();
            return;
        }
        // No hacer clamp aquí: queremos que morir suceda si se sale
        break;
    }
    case Collision::Type::HOME:
        // Ahogarse solo cuando estás parado (no en mitad del salto)
        if (collider.y < Game::RIVER_LOW) {
            resetToStart();
            return;
        }
        break;

    case Collision::Type::NONE:
    default:
        // Ahogarse solo cuando estás parado (no en mitad del salto)
        if (collider.y < Game::RIVER_LOW) {
            loseLife();
            resetToStart();
            return;
        }
        break;
    }
}

void Frog::handleEvents(const SDL_Event& e) {
    if (e.type == SDL_EVENT_KEY_DOWN && !e.key.repeat) {
        if (moving || jumpFrames > 0) return; // no aceptar mientras se mueve o anima

        int dx = 0, dy = 0;
        switch (e.key.key) {
        case SDLK_UP:    case SDLK_W: dy = -1; angle = 0.0f;   break;
        case SDLK_DOWN:  case SDLK_S: dy = 1; angle = 180.0f; break;
        case SDLK_LEFT:  case SDLK_A: dx = -1; angle = 270.0f; break;
        case SDLK_RIGHT: case SDLK_D: dx = 1; angle = 90.0f;  break;
        default: break;
        }
        if (dx == 0 && dy == 0) return;

        // Comprobar limites
        constexpr float UI_HEIGHT = 36.0f; // altura de la barra negra
        if (pos.getX() + dx * speed < 0.0f ||
            pos.getX() + dx * speed + texture->getFrameWidth()  >(float)Game::WINDOW_WIDTH ||
            pos.getY() + dy * speed < 0.0f ||
            pos.getY() + dy * speed + texture->getFrameHeight() >(float)Game::WINDOW_HEIGHT - UI_HEIGHT) {
            return; // el salto sacaría fuera ⇒ no iniciar
        }

        // Iniciar movimiento (sin fijar pos final para evitar “snap”)
        stepPerFrame = Vector2D<>{ (dx * speed) / (float)JUMP_FRAMES,
                                   (dy * speed) / (float)JUMP_FRAMES };
        moveFramesLeft = JUMP_FRAMES;
        moving = true;
        jumpFrames = JUMP_FRAMES;
    }
}
