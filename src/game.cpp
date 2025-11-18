#include "game.h"
#include "Wasp.h"
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
    {"turtle.png",1,7}
};

Game::Game() // constructor
{
    if (!SDL_Init(SDL_INIT_VIDEO))
        throw SDLError("SDL_Init");

    window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window)
        throw SDLError("SDL_CreateWindow");

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer)
        throw SDLError("SDL_CreateRenderer");

    for (size_t i = 0; i < textures.size(); ++i)
    {
        const TexSpec& spec = ::texList[i];
        string path = string(IMG_DIR) + spec.name;
        textures[i] = new Texture(renderer, path.c_str(), spec.rows, spec.cols);
    }
}


Game::~Game() // destructor
{
    for (SceneObject* o : objects) delete o;
    objects.clear();

    for (Texture*& t : textures) { delete t; t = nullptr; }

    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::initHomedFrogs() // inicializa nidos de HomedFrog
{
    if (homedSpawned) return;

    homedfrogs = 0;

    // Coloca las HomedFrog
    for (int i = 0; i < 5; ++i) {
        float x = float(firstH + i * spaceH);
        float y = float(houseY);
        addObject(new HomedFrog(this, textures[FROG], Point2D(x, y)));
    }

    homedSpawned = true;
}

void Game::reset() // reinicia partida
{
    for (SceneObject* o : objects) delete o;
    objects.clear();
    toDelete.clear();
    frog = nullptr;

    homedSpawned = false;
    initHomedFrogs();

    // Inicializa temporizador de avispas
    waspTimer = 0;
    nextWaspTime = getRandomRange(minTime, maxTime);

    loadMap(MAP_FILE);
}

void Game::loadMap(const char* path) // carga mapa desde archivo
{
    ifstream file(path);
    if (!file)
        throw FileNotFoundError(path);

    string line;
    int lineNum = 0;

    while (getline(file, line))
    {
        ++lineNum;

        if (line.empty() || line[0] == '#') continue;

        stringstream ss(line);
        char id; ss >> id;

        switch (id)
        {
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
        default:
            throw FileFormatError(path, lineNum, std::string("Unknown id: ") + id);
        }
    }
}


void Game::handleEvents() // manejo de eventos (principalmente reinicio, lo demas lo gestiona frog)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_EVENT_QUIT)
            exit = true;

        if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_0)
        {
            int buttonId = -1;

            const SDL_MessageBoxButtonData buttons[] = {
                { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Cancelar" },
                { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Reiniciar" }
            };

            const SDL_MessageBoxData msgData = {
                SDL_MESSAGEBOX_WARNING,            
                window,                            
                "Reiniciar partida",                
                "¿Seguro que quieres reiniciar la partida?", 
                SDL_arraysize(buttons),
                buttons,
                nullptr                              
            };

            if (SDL_ShowMessageBox(&msgData, &buttonId))
            {
                if (buttonId == 1)
                {
                    reset();
                    return;  
                }
            }
        }

        if (frog)
            frog->handleEvents(e);
    }
}

void Game::update() // actualización de todos los objetos
{
    SpawnWasps();

    for (auto it = objects.begin(); it != objects.end(); ++it)
        (*it)->update();

    flushDeletions();

    // Comprobar victoria: 5 nidos ocupados => fin de partida
    int occupied = 0;
    for (SceneObject* o : objects) {
        if (o->isHome() && o->isHomeOccupied()) ++occupied;
    }
    homedfrogs = occupied;
    if (homedfrogs >= 5) {
        exit = true; // saldrá del bucle en run()
    }
}

void Game::render() const // renderizado de todos los objetos
{
    SDL_RenderClear(renderer);

    textures[BACKGROUND]->render();

    for (SceneObject* o : objects)
        o->render();

    SDL_RenderPresent(renderer);
}

Collision Game::checkCollision(const SDL_FRect& box) const // colisiones de todos los objetos
{
    Collision c;
    for (SceneObject* o : objects)
    {
        if (o == frog) continue;

        c = o->checkCollision(box);
        if (c.type != Collision::Type::NONE)
            return c;
    }
    return c;
}


void Game::flushDeletions() // elimina objetos marcados para borrado (avispas caducadas)
{
    for (auto it : toDelete)
    {
        delete* it;
        objects.erase(it);
    }
    toDelete.clear();
}

void Game::run() // bucle principal del juego
{
	reset();

    while (!exit && frog && frog->getLives() > 0)
    {
        handleEvents();
        update();
        render();
        SDL_Delay(1000 / FRAME_RATE);
    }
}

void Game::SpawnWasps() // función de spawn de avispas
{
    // Spawn de avispas
    waspTimer++;
    if (waspTimer >= nextWaspTime) {
        waspTimer = 0;
        nextWaspTime = getRandomRange(minTime, maxSpawn);

		// busca y agrupa nidos libres (sin dynamic_cast)
        std::vector<SceneObject*> freeHomes;
        freeHomes.reserve(5);
        for (SceneObject* o : objects) {
            if (o->isHome() && !o->isHomeOccupied()) {
                freeHomes.push_back(o);
            }
        }

        // Si no hay nidos libres, no spawneamos ahora
        if (freeHomes.empty()) return;

        // Elige un nido libre aleatorio
        SceneObject* target = freeHomes[getRandomRange(0, (int)freeHomes.size() - 1)];

        const int waspW = textures[WASP]->getFrameWidth();
        const int waspH = textures[WASP]->getFrameHeight();

        SDL_FRect homeRect = target->getBoundingBox();
        const float x = homeRect.x + (homeRect.w - waspW) * 0.5f;
        const float y = homeRect.y + (homeRect.h - waspH) * 0.5f;

        Wasp* w = new Wasp(this, textures[WASP], Point2D(x, y), 2500 );
        Anchor an = addObject(w);
        w->setAnchor(an);
    }
}


