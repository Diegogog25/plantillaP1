#pragma once
#include "SceneObject.h"
#include "vector2D.h"
#include <SDL3/SDL.h>
#include "texture.h"
#include "Collision.h"
#include "game.h"


class Wasp : public SceneObject {
private:
    unsigned long long expireAtMs = 0;

    Game::Anchor anchor;

public:
    Wasp(Game* g, Texture* t, const Point2D& pos, Uint32 lifetimeMs);

    ~Wasp();

    void update() override;
    void render() const override { SceneObject::render(); }

    void setAnchor(Game::Anchor an) { anchor = an; }

    bool isAlive() const;
    void deleteWasp() { game->deleteAfter(anchor); }


    Collision checkCollision(const SDL_FRect& other) override;
};
