#pragma once

#include "GameState.h"
#include <list>

class SceneObject;
class Frog;
struct Collision;

/**
 * Estado de juego principal (partida).
 * En el futuro moveremos aquí casi todo lo que ahora está en Game:
 *  - carga de mapas
 *  - colisiones
 *  - spawn de avispas
 *  - etc.
 */
class PlayState : public GameState {
public:
    using SceneAnchor = std::list<SceneObject*>::iterator;

private:
    std::list<SceneObject*> sceneObjects;
    Frog* frog = nullptr; // rana principal

public:
    PlayState(Game* game, GameStateMachine* gameMachine);
    ~PlayState() override;

    void update() override;
    void render() const override;
    void handleEvents(const SDL_Event& e) override;

    // Gestión de objetos de escena
    SceneAnchor addSceneObject(SceneObject* o);
    void        removeSceneObject(SceneAnchor it);

    // Colisiones contra los objetos de escena
    Collision checkCollision(const SDL_FRect& box) const;

    Frog* getFrog() const { return frog; }
    void  setFrog(Frog* f) { frog = f; }
};

