#pragma once
#include "SceneObject.h"

class HomedFrog : public SceneObject {
private:
    bool inHouse = false;

public:
    HomedFrog(Game* g, Texture* t, Point2D pos);
    ~HomedFrog() override;

    void update() override;

    void setOccupied(bool v);
    bool isOccupied() const;

    void render() const override;
    Collision checkCollision(const SDL_FRect& rect) const override;
};
