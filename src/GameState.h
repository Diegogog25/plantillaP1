#pragma once
#include <list>
#include <SDL3/SDL.h>
class Game;
class GameStateMachine;
class EventHandler;
class GameObject;

class GameState {
protected:

	//Punteros
	Game* game;
	GameStateMachine* gameMachine;

	//Listas
	std::list<EventHandler*> listeners;
	std::list<GameObject*> gameObjects;

public:

	GameState(Game* game, GameStateMachine* gameMachine);
	virtual ~GameState();

	virtual void update() = 0;
	virtual void render()const = 0;
	void handleEvent(const SDL_Event& event);

	void addEventListener(EventHandler* listeners);
	void addObject(GameObject* gameObject);

	Game* getGame()const { return game };

};
