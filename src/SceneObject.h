#pragma once
#include "GameObject.h"
#include "texture.h"
#include "collision.h"
#include "vector2D.h"
#include <SDL3/SDL.h>

class SceneObject : public GameObject {
protected:
	Point2D pos;
    float w, h;
    Texture* tex;

    SDL_FRect bbox() const;

public:
    SceneObject(Game* g, Texture* t, Point2D pos, float W, float H);
    virtual ~SceneObject();

    void update() override;
    void render() const override;

    SDL_FRect getRect() const;

    virtual Collision checkCollision(const SDL_FRect& other) const;
};