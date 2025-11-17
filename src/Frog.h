#pragma once
#include "SceneObject.h"

class Frog : public SceneObject {
    // animación y control
    float angle = 0.f;
    int   jumpFrames = 0;
    static constexpr int JUMP_FRAMES = 6;
    bool  moving = false;
    int   framesLeft = 0;
    float stepX = 0.f, stepY = 0.f;

    int   lives = 3;
    float cell = 32.f;
    float uiH = 36.f; // barra superior
    float startX{}, startY{};

public:
    Frog(Game* g, Texture* t, float X, float Y)
        : SceneObject(g, t, pos, (float)t->getFrameWidth(), (float)t->getFrameHeight()),
        startX(X), startY(Y) {
    }

    void setLives(int n) { lives = n; }
    int  getLives() const { return lives; }
    void loseLife() { --lives; }

    void reset() { pos.setX(startX) ; pos.setY(startY); moving = false; framesLeft = 0; jumpFrames = 0; stepX = stepY = 0; }

    void handleEvents(const SDL_Event& e);
    void update() override;
    void render() const override;
};
