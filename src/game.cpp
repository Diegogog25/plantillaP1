#include "game.h"
#include "Wasp.h"
#include "MainMenuState.h"
#include "PlayState.h"
#include <fstream>
#include <sstream>
#include <string>
#include <SDL3_image/SDL_image.h>

using namespace std;

constexpr const char* WINDOW_TITLE = "Frogger P2";
constexpr const char* MAP_FILE = "../assets/maps/turtles.txt";
constexpr const char* IMG_DIR = "../assets/images/";

struct TexSpec { const char* name; int rows; int cols; };

constexpr array<TexSpec, Game::NUM_TEXTURES> texList{
    TexSpec{"frog.png", 1, 2},
    {"background.png", 1, 1},
    {"car1.png", 1, 1},
    {"car2.png", 1, 1},
    {"car3.png", 1, 1},
    {"car4.png", 1, 1},
    {"car5.png", 1, 1},
    {"log1.png", 1, 1},
    {"log2.png", 1, 1},
    {"wasp.png", 1, 1},
    {"turtle.png",1,7},

    //--UI--
    {"menuBackground.png", 1, 1},
    {"/texts/Avispado.png", 1, 1},
    {"/texts/CONTINUAR.png", 1, 1},
    {"/texts/ELIGE UN MAPA.png", 1, 1},
    {"/texts/GAME OVER.png", 1, 1},
    {"/texts/HAS GANADO.png", 1, 1},
    {"/texts/left.png", 1, 1},
    {"/texts/Original.png", 1, 1},
    {"/texts/Práctica 1.png", 1, 1},
    {"/texts/REINICIAR.png", 1, 1},
    {"/texts/right.png", 1, 1},
    {"/texts/SALIR.png", 1, 1},
    {"/texts/Trivial.png", 1, 1},
    {"/texts/Veloz.png", 1, 1},
    {"/texts/VOLVER AL MENÚ.png", 1, 1}
};

Game::Game()
{
    // SDL_Init devuelve 0 en éxito. Comprobar correctamente errores.
    if (!SDL_Init(SDL_INIT_VIDEO))
        throw SDLError("SDL_Init");

    window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window)
        throw SDLError("SDL_CreateWindow");

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer)
        throw SDLError("SDL_CreateRenderer");

    for (size_t i = 0; i < textures.size(); ++i) {
        const TexSpec& spec = ::texList[i];
        string path = string(IMG_DIR) + spec.name;
        textures[i] = new Texture(renderer, path.c_str(), spec.rows, spec.cols);
    }

    // Mapa por defecto
    selectedMap = MAP_FILE;
}

Game::~Game()
{
    for (SceneObject* o : objects) delete o;
    objects.clear();

    for (Texture*& t : textures) { delete t; t = nullptr; }

    if (renderer) SDL_DestroyRenderer(renderer);
    if (window)   SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::initHomedFrogs()
{
    if (homedSpawned) return;

    currentHomedFrogs = 0;

    for (int i = 0; i < 5; ++i) {
        float x = float(firstH + i * spaceH);
        float y = float(houseY);
        HomedFrog* homedFrog = new HomedFrog(this, textures[FROG], Point2D(x, y));
        addObject(homedFrog);
        homedFrogs.push_back(homedFrog);
    }

    homedSpawned = true;
}

void Game::reset()
{
    // Limpia escena y listas
    for (SceneObject* o : objects) delete o;
    objects.clear();
    toDelete.clear();
    frog = nullptr;

    // Reinicia nidos y contadores
    homedSpawned = false;
    occupied = 0;
    currentHomedFrogs = 0;
    homedFrogs.clear();
    initHomedFrogs();

    // Reinicia spawn de avispas
    waspTimer = 0;
    nextWaspTime = getRandomRange(minTime, maxTime);

    // Carga el mapa seleccionado
    loadMap(selectedMap.c_str());
}

void Game::loadMap(const char* path)
{
    ifstream file(path);
    if (!file)
        throw FileNotFoundError(path);

    string line;
    int lineNum = 0;

    while (getline(file, line)) {
        ++lineNum;
        if (line.empty() || line[0] == '#') continue;
        stringstream ss(line);
        char id; ss >> id;
        switch (id) {
        case 'F': {
            frog = Frog::FromMap(this, ss, path, lineNum);
            addObject(frog);
            break;
        }
        case 'V': {
            addObject(Vehicle::FromMap(this, ss, path, lineNum));
            break;
        }
        case 'L': {
            addObject(Log::FromMap(this, ss, path, lineNum));
            break;
        }
        case 'T': {
            addObject(TurtleGroup::FromMap(this, ss, path, lineNum));
            break;
        }
        case 'W': {
            Wasp* w = Wasp::FromMap(this, ss, path, lineNum);
            Anchor an = addObject(w);
            w->setAnchor(an);
            break;
        }
        default:
            throw FileFormatError(path, lineNum, std::string("Unknown id: ") + id);
        }
    }
}

void Game::update()
{
    // Lógica propia de la partida (invocada desde PlayState)
    SpawnWasps();

    for (auto it = objects.begin(); it != objects.end(); ++it)
        (*it)->update();

    flushDeletions();
    playstate->checkEnd();

}

void Game::render() const
{
    // Solo dibuja la escena; el clear/present lo gestiona el bucle principal/estados
    textures[BACKGROUND]->render();

    for (SceneObject* o : objects)
        o->render();
}

Collision Game::checkCollision(const SDL_FRect& box) const
{
    Collision c;
    for (SceneObject* o : objects) {
        if (o == frog) continue;

        c = o->checkCollision(box);
        if (c.type != Collision::Type::NONE)
            return c;
    }
    return c;
}


void Game::flushDeletions()
{
    for (auto it : toDelete) {
        delete* it;
        objects.erase(it);
    }
    toDelete.clear();
}
bool Game::checkVictory() {
    occupied = 0;
    for (HomedFrog* o : homedFrogs) {
        if (o->isOccupied()) ++occupied;
    }
    currentHomedFrogs = occupied;

    return currentHomedFrogs >= 5;
}

void Game::SpawnWasps()
{
    waspTimer++;
    if (waspTimer >= nextWaspTime) {
        waspTimer = 0;
        nextWaspTime = getRandomRange(minTime, maxSpawn);

        std::vector<SceneObject*> freeHomes;
        freeHomes.reserve(5);
        for (HomedFrog* o : homedFrogs) {
            if (!o->isOccupied()) {
                freeHomes.push_back(o);
            }
        }
        if (freeHomes.empty()) return;

        SceneObject* target = freeHomes[getRandomRange(0, (int)freeHomes.size() - 1)];

        const int waspW = textures[WASP]->getFrameWidth();
        const int waspH = textures[WASP]->getFrameHeight();

        SDL_FRect homeRect = target->getBoundingBox();
        const float x = homeRect.x + (homeRect.w - waspW) * 0.5f;
        const float y = homeRect.y + (homeRect.h - waspH) * 0.5f;

        Wasp* w = new Wasp(this, textures[WASP], Point2D(x, y), 2500);
        Anchor an = addObject(w);
        w->setAnchor(an);
    }
}
void Game::startPlayState() {
    auto ps = new PlayState(this, this);   
    replaceState(ps);                       
    playstate = ps;                         
}

void Game::run()
{
    const double targetFrameMs = 1000.0 / FRAME_RATE;
    const Uint64 perfFreq = SDL_GetPerformanceFrequency();

    exit = false;
    clearStartRequest();

    // Estado inicial: menú principal
    pushState(new MainMenuState(this, this));

    while (!exit && !empty()) {
        Uint64 frameStart = SDL_GetPerformanceCounter();

        // Entrada: delegada en el estado activo
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                while (!empty()) popState();
                exit = true;
                break;
            }
            GameStateMachine::handleEvents(e);
        }

        if (empty() || exit) break;

        // ¿El menú ha pedido empezar la partida?
        if (startRequested) {
            clearStartRequest();
            startPlayState();
        }

        // Lógica + render delegados
        GameStateMachine::update();

        SDL_RenderClear(renderer);
        GameStateMachine::render();
        SDL_RenderPresent(renderer);

        // Pacing de frames
        Uint64 frameEnd = SDL_GetPerformanceCounter();
        double elapsedMs = (frameEnd - frameStart) * 1000.0 / perfFreq;
        double sleepMs = targetFrameMs - elapsedMs;
        if (sleepMs > 0.0)
            SDL_Delay((Uint32)(sleepMs + 0.5));
    }


    while (!empty()) popState();
}


