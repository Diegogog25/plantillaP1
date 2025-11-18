#pragma once
#include "GameObject.h"
#include "texture.h"
#include "collision.h"
#include "vector2D.h"
#include <SDL3/SDL.h>

class SceneObject : public GameObject {
    friend class Game;
protected:
	Point2D pos;
    float w, h;
    Texture* tex;

    SDL_FRect getBoundingBox() const;

public:
    SceneObject(Game* g, Texture* t, Point2D pos, float W, float H);
    virtual ~SceneObject();

    void update() override;
    void render() const override;

    virtual Collision checkCollision(const SDL_FRect& other);

    virtual bool isHome() const { return false; }
    virtual bool isHomeOccupied() const { return false; }
};