#include "game.h"

#include <string>

#include <SDL3_image/SDL_image.h>

#include "texture.h"



using namespace std;

// Constantes
constexpr const char* const WINDOW_TITLE = "Frogger 1.0";
constexpr const char* const MAP_FILE = "../assets/maps/default.txt";

// Estructura para especificar las texturas que hay que
// cargar y el tamaño de su matriz de frames
struct TextureSpec
{
	const char* name;
	int nrows = 1;
	int ncols = 1;
};

constexpr const char* const imgBase = "../assets/images/";

constexpr array<TextureSpec, Game::NUM_TEXTURES> textureList{
	TextureSpec{"frog.png", 1, 2},
	{"background.png"},{"car1.png"},{"car2.png"},
	{"car3.png"},{"car4.png"},{"car5.png"},{"log1.png"},
	{"log2.png"}
};

Game::Game()
  : exit(false)
{
	// Carga SDL y sus bibliotecas auxiliares
	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow(WINDOW_TITLE,
	                          WINDOW_WIDTH,
	                          WINDOW_HEIGHT,
	                          0);

	if (window == nullptr)
		throw "window: "s + SDL_GetError();

	renderer = SDL_CreateRenderer(window, nullptr);

	if (renderer == nullptr)
		throw "renderer: "s + SDL_GetError();

	// Carga las texturas al inicio
	for (size_t i = 0; i < textures.size(); i++) {
		auto [name, nrows, ncols] = textureList[i];
		textures[i] = new Texture(renderer, (string(imgBase) + name).c_str(), nrows, ncols);
	}

	// Configura que se pueden utilizar capas translúcidas
	// SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

Game::~Game()
{
	// TODO: liberar memoria reservada por la clase
}

void Game::render() const
{
	SDL_RenderClear(renderer);

	// TODO
	SDL_FRect destRect{0,0,WINDOW_WIDTH, WINDOW_HEIGHT};
	textures[1]->render(destRect);
	for (int i = 0;i < vehicles.size();i++) vehicles[i]->render();


	for (int i = 0;i < logs.size();i++) logs[i]->render();

	SDL_RenderPresent(renderer);	
}

void
Game::update()
{
	// TODO
	for (int i = 0;i < vehicles.size();i++) vehicles[i]->update();
	for (int i = 0;i < logs.size();i++) logs[i]->update();
}

void
Game::run()
{
	Point2D point = { 200.0f,200.0f };
	Point2D _point = { 100.0f,100.0f };
	Vector2D<> v = { 5.0f, 0.0f };
	Texture* t = textures[2];
	Texture* _t = textures[8];

	Vehicle veh(t, point, v);
	vehicles.push_back(&veh);

	Log l(_t, _point, v);
	logs.push_back(&l);

	
	while (!exit) {
		// TODO: implementar bucle del juego
		update();
		render();
		SDL_Delay(1000 / FRAME_RATE);
	}
}

void
Game::handleEvents()
{
	SDL_Event event;

	// Only quit is handled directly, everything else is delegated
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT)
			exit = true;
		
		// TODO

	}
}

bool
Game::checkCollision(const SDL_FRect& rect) const
{
	// TODO: cambiar el tipo de retorno a Collision e implementar
	return false;
}
