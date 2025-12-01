#pragma once

#include "GameObject.h"
#include "texture.h"
#include <SDL3/SDL.h>

class Label : public GameObject {
protected:
    Texture* texture = nullptr;
    SDL_FRect rect{};

public:
    Label(Game* game, Texture* tex, const SDL_FRect& rect);
    ~Label() override;

    const SDL_FRect& getRect() const { return rect; }
    void setPosition(float x, float y) { rect.x = x; rect.y = y; }
    void setSize(float w, float h) { rect.w = w; rect.h = h; }

    void setTexture(Texture* tex) { texture = tex; }

    void render() const override;

};
