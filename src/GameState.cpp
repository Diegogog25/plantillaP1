#include "GameState.h"
#include "GameObject.h"
#include "EventHandler.h"

GameState::GameState(Game* g, GameStateMachine* gsm)
    : game(g), gameMachine(gsm)
{
}

GameState::~GameState()
{
    // GameState es propietario de sus GameObject
    for (GameObject* obj : gameObjects) {
        delete obj;
    }
    gameObjects.clear();
}

void GameState::render() const
{
    for (GameObject* obj : gameObjects) {
        obj->render();
    }
}

void GameState::update()
{
    for (GameObject* obj : gameObjects) {
        obj->update();
    }

    // Ejecutar callbacks diferidos
    for (DelayedCallback& cb : pendingCallbacks) {
        if (cb) cb();
    }
    pendingCallbacks.clear();
}

void GameState::handleEvents(const SDL_Event& event)
{
    for (auto handler : listeners) {
        handler->handleEvent(event);
    }
}

GameState::Anchor GameState::addObject(GameObject* obj)
{
    gameObjects.push_back(obj);
    return std::prev(gameObjects.end());
}

void GameState::removeObject(Anchor it)
{
    gameObjects.erase(it);
}

void GameState::addEventListener(EventHandler* listener)
{
    listeners.push_back(listener);
}

void GameState::runLater(DelayedCallback cb)
{
    pendingCallbacks.push_back(std::move(cb));
}
