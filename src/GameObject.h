#pragma once
#include <SDL3/SDL.h>

class Game;

class GameObject {
protected:
    Game* game;
    GameObject(Game* g);
public:

    virtual ~GameObject();

    virtual void render() const = 0;
    virtual void update() = 0;
};
