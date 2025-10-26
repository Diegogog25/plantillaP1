#include "game.h"

#include <string>
#include <SDL3_image/SDL_image.h>

// ----------------- Config de recursos -----------------

using namespace std;

constexpr const char* const WINDOW_TITLE = "Frogger 1.0";
constexpr const char* const MAP_FILE = "../assets/maps/default.txt";

// Estructura para especificar las texturas que hay que cargar
struct TextureSpec
{
    const char* name;
    int nrows = 1;
    int ncols = 1;
};

constexpr const char* const imgBase = "../assets/images/";

constexpr array<TextureSpec, Game::NUM_TEXTURES> textureList{
    TextureSpec{"frog.png", 1, 2},
    {"background.png"},
    {"car1.png"},
    {"car2.png"},
    {"car3.png"},
    {"car4.png"},
    {"car5.png"},
    {"log1.png"},
    {"log2.png"}
};

// ----------------- Implementación Game -----------------

Game::Game()
    : exit(false)
{
    // Carga SDL y crea ventana y renderer
    if (!SDL_Init(SDL_INIT_VIDEO))
        throw "SDL_Init: "s + SDL_GetError();

    window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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

    // Config opcional de blending
    // SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

Game::~Game()
{
    // Libera objetos del juego creados con new
    for (auto* v : vehicles) delete v;
    vehicles.clear();

    for (auto* l : logs) delete l;
    logs.clear();

    for (auto* w : wasps) delete w;
    wasps.clear();

    for (auto* h : homed) delete h;
    homed.clear();

    if (frog) { delete frog; frog = nullptr; }

    // Libera texturas
    for (auto*& t : textures) { delete t; t = nullptr; }

    // Destruye SDL
    if (renderer) { SDL_DestroyRenderer(renderer); renderer = nullptr; }
    if (window) { SDL_DestroyWindow(window); window = nullptr; }
    SDL_Quit();
}

void Game::render() const
{
    SDL_RenderClear(renderer);

    // Fondo
    SDL_FRect bg{ 0, 0, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT };
    textures[BACKGROUND]->render(bg);

    // Objetos
    for (auto* v : vehicles) v->render();
    for (auto* l : logs)     l->render();
    for (auto* w : wasps)    w->render();
    for (auto* h : homed)    h->render();
    if (frog) frog->render();

    SDL_RenderPresent(renderer);
}

void Game::update()
{
    // Actualiza móviles
    for (auto* v : vehicles) v->update();
    for (auto* l : logs)     l->update();
    for (auto* w : wasps)    w->update();
    if (frog) frog->update();

    // Elimina avispas caducadas (delete + erase)
    for (size_t i = 0; i < wasps.size(); /*i++ dentro*/) {
        if (!wasps[i]->isAlive()) {
            delete wasps[i];
            wasps.erase(wasps.begin() + i);
        }
        else {
            ++i;
        }
    }
}

void Game::run()
{
    // -----------------------------------------------------------------
    // Construcción temporal de objetos de prueba (hasta que cargues el mapa)
    // ¡OJO! No crear locales y meter &puntero: hay que reservar con new.
    // -----------------------------------------------------------------
    frog = new Frog(textures[FROG], Point2D{ 200.f, 440.f }, this);

    vehicles.push_back(new Vehicle(
        textures[CAR2], Point2D{ 100.f, 350.f }, Vector2D<>{2.f, 0.f}));

    logs.push_back(new Log(
        textures[LOG2], Point2D{ 50.f, 120.f }, Vector2D<>{1.5f, 0.f}));

    // Avispa de test (5 segundos de vida)
    wasps.push_back(new Wasp(
        textures[CAR5], Point2D{ 300.f, 260.f }, Vector2D<>{0.f, 0.f}, 5000));

    // homed de test (una rana ya en casa)
    homed.push_back(new HomedFrog(textures[FROG], Point2D{ 32.f, 16.f }));

    // -----------------------------------------------------------------

    while (!exit) {
        handleEvents();
        update();
        render();
        SDL_Delay(1000 / FRAME_RATE);
    }
}

void Game::handleEvents()
{
    SDL_Event event;

    // Solo quit se maneja aquí; el resto se delega a la rana
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT)
            exit = true;

        if (frog)
            frog->handleEvents(event);
    }
}

Collision Game::checkCollision(const SDL_FRect& rect) const
{
    // Orden sugerida en el enunciado:
    // primero ENEMY (vehículos, avispas, ranas en casa),
    // luego PLATFORM (troncos con velocidad).
    for (auto* v : vehicles) {
        Collision c = v->checkCollision(rect);
        if (c.type != Collision::Type::NONE) return c;
    }
    for (auto* w : wasps) {
        Collision c = w->checkCollision(rect);
        if (c.type != Collision::Type::NONE) return c;
    }
    for (auto* h : homed) {
        Collision c = h->checkCollision(rect);
        if (c.type != Collision::Type::NONE) return c;
    }
    for (auto* l : logs) {
        Collision c = l->checkCollision(rect);
        if (c.type != Collision::Type::NONE) return c;
    }
    return {};
}
