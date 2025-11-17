#include "game.h"
#include <string>
#include <fstream>
#include <sstream>

// SDL3_image: sólo hace falta el include, sin IMG_Init/IMG_Quit
#include <SDL3_image/SDL_image.h>

using namespace std;

constexpr const char* WINDOW_TITLE = "Frogger P2";
constexpr const char* MAP_FILE = "../assets/maps/default.txt";
constexpr const char* IMG_DIR = "../assets/images/";

struct TexSpec { const char* name; int rows = 1, cols = 1; };
constexpr array<TexSpec, Game::NUM_TEXTURES> texList{
    TexSpec{"frog.png",1,2},
    {"background.png"},
    {"car1.png"},{"car2.png"},{"car3.png"},{"car4.png"},{"car5.png"},
    {"log1.png"},{"log2.png"},
    {"wasp.png"}
};

Game::Game()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        throw std::string("SDL_Init: ") + SDL_GetError();

    // OJO: en SDL3_image ya NO se llama a IMG_Init.
    // Basta con incluir SDL_image y usar IMG_LoadTexture en Texture.

    window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window)
        throw std::string("SDL_CreateWindow: ") + SDL_GetError();

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer)
        throw std::string("SDL_CreateRenderer: ") + SDL_GetError();

    for (size_t i = 0; i < textures.size(); ++i) {
        auto [n, r, c] = texList[i];
        textures[i] = new Texture(renderer, (std::string(IMG_DIR) + n).c_str(), r, c);
    }

    // aquí lo que ya tuvieras: reset(), loadMap(), etc.
}


Game::~Game()
{
    // Libera objetos de juego
    for (auto* o : objects) delete o;
    objects.clear();
    toDelete.clear();
    frog = nullptr;

    // Libera texturas
    for (auto*& t : textures) { delete t; t = nullptr; }

    if (renderer) { SDL_DestroyRenderer(renderer); renderer = nullptr; }
    if (window) { SDL_DestroyWindow(window);     window = nullptr; }

    // En SDL3_image no se llama a IMG_Quit; sólo SDL_Quit.
    SDL_Quit();
}


void Game::reset() {
    // borra objetos y recarga mapa
    for (auto* o : objects) delete o;
    objects.clear(); toDelete.clear(); frog = nullptr;

    loadMap(MAP_FILE);
    waspTimer = getRandomRange(20, 50);
}

void Game::loadMap(const char* path) {
    ifstream in(path);
    if (!in) throw FileNotFoundError(path);

    string line; int lineNo = 0;
    while (getline(in, line)) {
        ++lineNo;
        std::istringstream iss(line);
        iss >> std::ws;
        if (!iss.good() || iss.peek() == '#') continue;

        char id; if (!(iss >> id)) continue;
        switch (id) {
        case 'V': {
            float x, y, vx; int type;
            if (!(iss >> x >> y >> vx >> type)) throw FileFormatError(path, lineNo, "Vehicle V x y vx type");
            Texture* t = (type == 1) ? textures[CAR1] : (type == 2) ? textures[CAR2] : (type == 3) ? textures[CAR3] : (type == 4) ? textures[CAR4] : textures[CAR5];
            addObject(new Vehicle(this, t, x, y, vx / FRAME_RATE));
        } break;
        case 'L': {
            float x, y, vx; int kind = 0;
            if (!(iss >> x >> y >> vx)) throw FileFormatError(path, lineNo, "Log L x y vx [kind]");
            if (iss >> kind) { /* opcional */ }
            Texture* t = (kind == 1) ? textures[LOG2] : textures[LOG1];
            addObject(new Log(this, t, x, y, vx / FRAME_RATE));
        } break;
        case 'T': {
            float x, y, vx; int n, h;
            if (!(iss >> x >> y >> vx >> n >> h)) throw FileFormatError(path, lineNo, "T x y vx n h");
            // anchura/altura de UNA tortuga = frame de textura de log2 (o una textura de tortuga si la tienes)
            float W = (float)textures[LOG2]->getFrameWidth();
            float H = (float)textures[LOG2]->getFrameHeight();
            addObject(new TurtleGroup(this, textures[LOG2], x, y, W, H, vx / FRAME_RATE, 0.f, (float)WINDOW_WIDTH, n, h != 0));
        } break;
        case 'F': {
            float x, y; int lives = 3;
            if (!(iss >> x >> y)) throw FileFormatError(path, lineNo, "F x y [lives]");
            if (iss >> lives) {}
            frog = new Frog(this, textures[FROG_TEX], x, y);
            frog->setLives(lives);
            addObject(frog);
        } break;
        default:
            throw FileFormatError(path, lineNo, std::string("Unknown id '") + id + "'");
        }
    }

    // Crea 5 “nidos” visuales (HomedFrog invisibles inicialmente)
    for (int i = 0; i < 5; ++i) {
        auto hf = new HomedFrog(this, textures[FROG_TEX], (float)(firstH - 6 + spaceH * i), (float)(houseY - 6));
        addObject(hf);
    }
}

void Game::flushDeletions() {
    for (auto it : toDelete) { delete* it; objects.erase(it); }
    toDelete.clear();
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {

        if (e.type == SDL_EVENT_QUIT)
            exit = true;

        // Reinicio con tecla R
        if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_R) {
            reset();
            return; // evitar procesar eventos antiguos
        }

        // Pasar eventos a la rana
        if (frog)
            frog->handleEvents(e);
    }
}


Collision Game::checkCollision(const SDL_FRect& box) const {
    // 1ª pasada: ENEMY/HOME
    for (auto* o : objects) {
        Collision c = o->checkCollision(box);
        if (c.type == Collision::Type::ENEMY || c.type == Collision::Type::HOME)
            return c;
    }
    return {};
}
Collision Game::checkPlatform(const SDL_FRect& box) const {
    // 2ª pasada: PLATFORM
    for (auto* o : objects) {
        Collision c = o->checkCollision(box);
        if (c.type == Collision::Type::PLATFORM) return c;
    }
    return {};
}

void Game::update() {
    // actualizar todos
    for (auto it = objects.begin(); it != objects.end(); ++it) {
        (*it)->update();
        // si es Wasp y caduca → elimina al final
        if (auto* w = dynamic_cast<Wasp*>(*it)) {
            if (!w->isAlive()) deleteAfter(it);
        }
        // si la rana ha llegado a HOME, marca nido ocupado
        if (auto* f = dynamic_cast<Frog*>(*it)) {
            // busca colisión HOME rápida (usando su bbox actual)
            SDL_FRect box{ f->getRect().x, f->getRect().y, f->getRect().w, f->getRect().h };
            for (auto* o : objects) {
                if (auto* hf = dynamic_cast<HomedFrog*>(o)) {
                    if (!hf->isOccupied() && SDL_HasRectIntersectionFloat(&hf->getRect(), &box)) {
                        hf->setOccupied(true);
                        break;
                    }
                }
            }
        }
    }

    // Spawner simple de avispas
    if (--waspTimer <= 0) {
        // busca nidos libres
        std::vector<int> libres;
        for (int i = 0; i < 5; ++i) {
            float nx = (float)(firstH + spaceH * i);
            float ny = (float)houseY;
            // si ya hay HomedFrog ocupada ahí, no spawnear
            bool ocupado = false;
            for (auto* o : objects) {
                if (auto* hf = dynamic_cast<HomedFrog*>(o)) {
                    if (hf->isOccupied() &&
                        std::fabs(hf->getRect().x - (nx - 6)) < 2.f &&
                        std::fabs(hf->getRect().y - (ny - 6)) < 2.f) {
                        ocupado = true; break;
                    }
                }
            }
            if (!ocupado) libres.push_back(i);
        }
        if (!libres.empty()) {
            int idx = libres[getRandomRange(0, (int)libres.size() - 1)];
            float sx = (float)(firstH + spaceH * idx);
            float sy = (float)houseY;
            auto it = addObject(new Wasp(this, textures[WASP], sx, sy, (unsigned)getRandomRange(3000, 7000)));
            (void)it; // (el borrado se hace por caducidad)
        }
        waspTimer = getRandomRange(minTime, maxSpawn);
    }

    flushDeletions();
}

void Game::render() const {
    SDL_RenderClear(renderer);
    // Fondo
    textures[BACKGROUND]->render(SDL_FRect{ 0,0,(float)WINDOW_WIDTH,(float)WINDOW_HEIGHT });
    for (auto* o : objects) o->render();
    SDL_RenderPresent(renderer);
}

void Game::run() {
    loadMap(MAP_FILE);
    waspTimer = getRandomRange(20, 50);

    while (!exit && frog && frog->getLives() > 0) {
        handleEvents();
        update();
        render();
        SDL_Delay(1000 / FRAME_RATE);
    }
}
