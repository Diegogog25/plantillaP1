#pragma once
#include "vector2D.h"
#include "texture.h"
#include "SDL3/SDL.h"

class Game;

class Frog
{
    Texture* texture = nullptr;
    Point2D pos;
    Point2D startPos; // posición inicial para reset al morir
    Game* game = nullptr;

    // Dirección solicitada (solo se usa para iniciar un movimiento)
    Vector2D<> dir{ 0,0 };
    float speed = 32.f; // distancia por paso (una "casilla")

    int lives = 3;

    // Orientación y animación
    float angle = 0.f;
    int   jumpFrames = 0;
    static constexpr int JUMP_FRAMES = 6; // también duración del paso suave (6 frames = 200 ms a 30 fps)

    // Movimiento suave por paso (no se puede recalcular hasta terminar)
    bool moving = false;
    int  moveFramesLeft = 0;
    Vector2D<> stepPerFrame{0.f, 0.f};
    Point2D moveEnd;

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
    void setLives(int n) { lives = n; } // requerido por game.cpp

    // Resetea la rana a la posición inicial
    void resetToStart() { pos = startPos; dir = {0,0}; moving = false; moveFramesLeft = 0; stepPerFrame = {0,0}; }
};