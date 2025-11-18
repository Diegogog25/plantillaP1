#pragma once
#include "Crosser.h"
#include <iosfwd> // std::istream

class Vehicle : public Crosser {
public:
    Vehicle(Game* g, Texture* tex,
        Point2D pos,
        float vx,
        float leftSpan, float rightSpan);

    static SceneObject* FromMap(Game* g, std::istream& ss, const char* path, int lineNum);

    ~Vehicle() override;

    void render() const override;
    Collision checkCollision(const SDL_FRect& other) override;
};