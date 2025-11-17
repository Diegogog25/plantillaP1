#pragma once

class Game;   // Declaración adelantada

class GameObject {
protected:
    Game* game;

public:
    explicit GameObject(Game* g);

    virtual ~GameObject();

    virtual void render() const = 0;
    virtual void update() = 0;
};
