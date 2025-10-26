#pragma once
#include "vector2D.h"
#include "texture.h"
#include "SDL3/SDL.h"

class Game;

class Frog
{
    Texture* texture = nullptr;
    Point2D pos;
    Game* game = nullptr;

    // Dirección discreta (para movimiento por casillas)
    Vector2D<> dir{ 0,0 };
    float speed = 30.f; // tamaño de casilla

    int lives = 3;

public:
    Frog(Texture* _texture, Point2D _pos, Game* _game);
    ~Frog();

    void render() const;
    void update();
    void handleEvents(const SDL_Event& e);

    const Point2D& getPos() const { return pos; }
    void setPos(Point2D p) { pos = p; }

    int getLives() const { return lives; }
    void loseLife() { --lives; }
};
