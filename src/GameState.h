#pragma once

#include <list>
#include <functional>
#include <SDL3/SDL.h>

class Game;
class GameStateMachine;
class EventHandler;
class GameObject;

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

    virtual void update();
    virtual void render() const;
    virtual void handleEvents(const SDL_Event& event);

    Anchor addObject(GameObject* obj);
    void   removeObject(Anchor it);

    void addEventListener(EventHandler* listener);

    // Ejecuta una tarea al final del update en el que este
    void runLater(DelayedCallback cb);

    Game* getGame() const { return game; }
};
