#pragma once
#include "Platform.h"
#include <iosfwd>

class Game;
class Texture;

class TurtleGroup : public Platform {
private:
    int  numTurtles;
    bool submerging;
    int  timer = 0;
    int  period = 180;
    bool sunk = false;

public:
    TurtleGroup(Game* g, Texture* tex,
        Point2D pos,
        float w, float h,
        float vx,
        float leftSpan, float rightSpan,
        int n, bool sink);

    static SceneObject* FromMap(Game* g, std::istream& ss, const char* path, int lineNum);

    ~TurtleGroup() override = default;

    void update() override;
    void render() const override;
    Collision checkCollision(const SDL_FRect& other) override;
};