#pragma once
#include "SceneObject.h"
#include "vector2D.h"

class Crosser : public SceneObject
{
protected:
    Vector2D<> vel;
    float leftLimit;
    float rightLimit;

public:
    Crosser(Game* g, Texture* tex,
        float X, float Y,
        float W, float H,
        Vector2D<> v,
        float left, float right);

    ~Crosser() override = default;

    void update() override ;

    const Vector2D<>& getVel() const;
};
