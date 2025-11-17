#pragma once
class Game;   // declaración adelantada

struct GameObject {
    Game* game;
    explicit GameObject(Game* g) : game(g) {}
    virtual ~GameObject() = default;

    virtual void render() const = 0;
    virtual void update() = 0;
};
