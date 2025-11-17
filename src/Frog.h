#pragma once
#include "SceneObject.h"

class Frog : public SceneObject {
    // animación y control
    float angle = 0.f;
    int   jumpFrames = 0;
    static constexpr int JUMP_FRAMES = 6;
    bool  moving = false;
    int   framesLeft = 0;
    Vector2D<> step = { 0, 0 };

    int   lives = 3;
    float cell = 32.f;
    float uiH = 36.f; // barra superior
	Point2D startPos;

public:
    Frog(Game* g, Texture* t, Point2D pos)
        : SceneObject(g, t, pos, (float)t->getFrameWidth(), (float)t->getFrameHeight()),
        startPos(pos) {
    }

    void setLives(int n) { lives = n; }
    int  getLives() const { return lives; }
    void loseLife() { --lives; }

    void reset() { pos.setX(startPos.getX()); pos.setY(startPos.getY()); moving = false; framesLeft = 0; jumpFrames = 0; step.setX(0); step.setY(0); }

    void handleEvents(const SDL_Event& e);
    void update() override;
    void render() const override;
};
