#include "Frog.h"

Frog::Frog(Texture* _texture, Point2D _pos, Game* _game) {
	texture = _texture;
	pos = _pos;
	game = _game;

}
Frog::~Frog() {

}

void Frog::render()const {
	SDL_FRect destRect{ pos.getX(), pos.getY(), texture->getFrameWidth(), texture->getFrameHeight() };
	texture->render(destRect);
}
void Frog::update() {

	
}
void Frog::handleEvents(const SDL_Event& _input) {

	if (_input.type == SDL_EVENT_KEY_DOWN) {
		switch (_input.key.key) {
		case SDLK_UP : case  SDLK_W :
			if (pos.getY() - speed >= 0) pos = pos + Vector2D<>{0, -speed};
			break;
		case SDLK_DOWN : case SDLK_S :
			if (pos.getY() + speed <= 0) pos = pos + Vector2D<>{0, speed};
			break;
		case SDLK_RIGHT : case SDLK_D:
			if (pos.getY() + speed <= 0) pos = pos + Vector2D<>{speed, 0};
			break;
		case SDLK_LEFT : case SDLK_A:
			if (pos.getY() + speed <= 0) pos = pos + Vector2D<>{-speed, 0};
			break;

		}
	}
}
