#pragma once

#include <list>
#include <functional>
#include <SDL3/SDL.h>

class Game;
class GameStateMachine;
class EventHandler;
class GameObject;

/**
 * Estado base del juego.
 * Mantiene:
 *  - puntero al Game
 *  - puntero a la máquina de estados
 *  - lista de GameObject* (propiedad del estado)
 *  - lista de EventHandler* (oyentes de eventos)
 *  - lista de callbacks diferidos (DelayedCallback)
 */
class GameState {
public:
    using Anchor = std::list<GameObject*>::iterator;
    using DelayedCallback = std::function<void()>;

protected:
    Game* game = nullptr;
    GameStateMachine* gameMachine = nullptr;

    std::list<EventHandler*> listeners;
    std::list<GameObject*>   gameObjects;
    std::list<DelayedCallback> pendingCallbacks;

public:
    GameState(Game* game, GameStateMachine* gameMachine);
    virtual ~GameState();

    // Implementación por defecto: actualiza y pinta todos los objetos,
    // y ejecuta los callbacks diferidos al final.
    virtual void update();
    virtual void render() const;
    virtual void handleEvents(const SDL_Event& event);

    // Gestión de objetos del estado
    Anchor addObject(GameObject* obj);
    void   removeObject(Anchor it);

    // Gestión de oyentes
    void addEventListener(EventHandler* listener);

    // Ejecutar una tarea al final del update actual
    void runLater(DelayedCallback cb);

    Game* getGame() const { return game; }
};
