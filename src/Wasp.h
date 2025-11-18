#pragma once
#include "game.h"        // <-- para usar Game::Anchor
#include "SceneObject.h"
#include <SDL3/SDL.h>
#include <list>
#include "texture.h"
#include "collision.h"

class Wasp : public SceneObject {
private:
    unsigned long long expireAtMs = 0;
	Game::Anchor anchor;   // alias para el iterador del objeto en la lista polimórfica del juego

public:
    Wasp(Game* g, Texture* t, const Point2D& pos, Uint32 lifetimeMs);
    ~Wasp() override;

    void update() override;
    void render() const override;

    void setAnchor(Game::Anchor an) { anchor = an; } // usa el alias

    bool isAlive() const;
    void deleteWasp();

    Collision checkCollision(const SDL_FRect& other) override;
};