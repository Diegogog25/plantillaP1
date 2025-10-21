#pragma once
#include "vector2D.h"
#include "texture.h"
#include "SDL3/SDL.h"

class Game;

class Frog
{
private:
	Texture* texture;
	Point2D pos;
	Game* game;
	const float speed = 5;

public:
	Frog(Texture* _texture, Point2D _pos, Game* _game);
	~Frog();
	void handleEvents(const SDL_Event& _input);

	void render()const;
	void update();

};

