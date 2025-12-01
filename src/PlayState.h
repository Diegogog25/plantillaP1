#pragma once

#include "GameState.h"
#include <list>

class SceneObject;
class Frog;
struct Collision;

class PlayState : public GameState {
private:
    std::list<SceneObject*> sceneObjects;
    Frog* frog = nullptr; // rana principal


public:
    using SceneAnchor = std::list<SceneObject*>::iterator;
    PlayState(Game* game, GameStateMachine* gameMachine);
    ~PlayState() override;

    void update() override;
    void render() const override;
    void handleEvents(const SDL_Event& e) override;

    // Gestión de objetos
    SceneAnchor addSceneObject(SceneObject* o);
    void removeSceneObject(SceneAnchor it);

    // Colisiones 
    Collision checkCollision(const SDL_FRect& box) const;

    void checkEnd();

    Frog* getFrog() const { return frog; }
    void  setFrog(Frog* f) { frog = f; }
};

