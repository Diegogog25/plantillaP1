#include "GameState.h"
#include "GameObject.h"
#include "gameStateMachine.h"
#include "Game.h"


void GameState::render()const {
	for (GameObject* obj : gameObjects) {
		obj->render();
	}
}

void GameState::update() {
	for (GameObject* obj : gameObjects) {
		obj->update();
	}
}

void GameState::handleEvent(const SDL_Event& event) {
	for (auto handler : listeners)
		handler->handleEvent(event);

}
void GameState::addObject(GameObject* obj)
{
	gameObjects.push_back(obj);
}

void GameState::addEventListener(EventHandler* event)
{
	listeners.push_back(event);
}
