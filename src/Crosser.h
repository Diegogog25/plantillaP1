#pragma once
#include "SceneObject.h"
#include "vector2D.h"

class Crosser : public SceneObject
{
protected:
    Vector2D<> vel;
    Point2D pos;
    float leftLimit;
    float rightLimit;

public:
    Crosser(Game* g, Texture* tex,
		Point2D pos,
        float W, float H,
        Vector2D<> v,
        float left, float right);

    ~Crosser() override = default;

    void update() override ;

    const Vector2D<>& getVel() const;
};
