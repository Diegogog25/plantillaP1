#pragma once
#include "game.h"        // para usar Game::Anchor
#include "SceneObject.h"
#include <SDL3/SDL.h>
#include <list>
#include "texture.h"
#include "collision.h"
#include <iosfwd>

class Wasp : public SceneObject {
private:
    unsigned long long expireAtMs = 0;
    Game::Anchor anchor;

public:
    Wasp(Game* g, Texture* t, const Point2D& pos, Uint32 lifetimeMs);
    ~Wasp() override;

    void update() override;
    void render() const override;

    void setAnchor(Game::Anchor an) { anchor = an; }

    bool isAlive() const;
    void deleteWasp();

    Collision checkCollision(const SDL_FRect& other) override;

    // Ahora devuelve Wasp* (no SceneObject*)
    static Wasp* FromMap(Game* g, std::istream& ss, const char* path, int lineNum);
};