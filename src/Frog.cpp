#include "Frog.h"
#include "game.h"

Frog::Frog(Texture* _texture, Point2D _pos, Game* _game) {
    texture = _texture;
    pos = _pos;
    game = _game;
}
Frog::~Frog() {}

void Frog::render() const {
    SDL_FRect destRect{ pos.getX(), pos.getY(), (float)texture->getFrameWidth(), (float)texture->getFrameHeight() };
    texture->render(destRect);
}

void Frog::update() {
    // movimiento discreto por la última dirección establecida
    pos = pos + dir;
    // opcional: volver a dejar la dirección a 0 si quieres un “paso” por tecla
    dir = { 0,0 };
}

void Frog::handleEvents(const SDL_Event& e) {
    if (e.type == SDL_EVENT_KEY_DOWN) {
        switch (e.key.key) {
        case SDLK_UP: case SDLK_W:
            if (pos.getY() - speed >= 0) dir = Vector2D<>{ 0, -speed };
            break;
        case SDLK_DOWN: case SDLK_S:
            if (pos.getY() + speed <= Game::WINDOW_HEIGHT - texture->getFrameHeight())
                dir = Vector2D<>{ 0, speed };
            break;
        case SDLK_RIGHT: case SDLK_D:
            if (pos.getX() + speed <= Game::WINDOW_WIDTH - texture->getFrameWidth())
                dir = Vector2D<>{ speed, 0 };
            break;
        case SDLK_LEFT: case SDLK_A:
            if (pos.getX() - speed >= 0) dir = Vector2D<>{ -speed, 0 };
            break;
        }
    }
}
