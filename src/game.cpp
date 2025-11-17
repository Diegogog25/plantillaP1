#include "game.h"
#include <fstream>
#include <sstream>
#include <string>
#include <SDL3_image/SDL_image.h>

using namespace std;

constexpr const char* WINDOW_TITLE = "Frogger P2";
constexpr const char* MAP_FILE = "../assets/maps/default.txt";
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
    {"wasp.png", 1, 1}
};

Game::Game()
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
        auto [name, rows, cols] = ::texList[i];
        string path = string(IMG_DIR) + name;
        textures[i] = new Texture(renderer, path.c_str(), rows, cols);
    }
}


Game::~Game()
{
    for (auto* o : objects) delete o;
    objects.clear();

    for (auto*& t : textures) { delete t; t = nullptr; }

    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::reset()
{
    for (auto* o : objects) delete o;
    objects.clear();
    toDelete.clear();
    frog = nullptr;

    loadMap(MAP_FILE);
}

void Game::loadMap(const char* path)
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
        char id;
        ss >> id;

        float x, y, vx;
		Point2D pos;
        int type = 1; // valor por defecto para logs

        float leftSpan = -150.0f;
        float rightSpan = WINDOW_WIDTH + 150.0f;

        switch (id)
        {
        case 'F': {
            if (!(ss >> x >> y))
                throw FileFormatError(path, lineNum, "Invalid Frog line");
			pos.setX(x);
			pos.setY(y);
            frog = new Frog(this, textures[FROG], pos);
            addObject(frog);
            break;
        }

        case 'V': {
            if (!(ss >> x >> y >> vx >> type))
                throw FileFormatError(path, lineNum, "Invalid Vehicle line");

            Texture* t =
                (type == 1) ? textures[CAR1] :
                (type == 2) ? textures[CAR2] :
                (type == 3) ? textures[CAR3] :
                (type == 4) ? textures[CAR4] :
                textures[CAR5];

			pos.setX(x);
			pos.setY(y);
            addObject(
                new Vehicle(this, t,
                    pos,
                    vx / FRAME_RATE,
                    leftSpan, rightSpan)
            );
            break;
        }

        case 'L': {
            if (!(ss >> x >> y >> vx))   // SIN type en el mapa original
                throw FileFormatError(path, lineNum, "Invalid Log line");

            // Si quieres soportar un campo opcional:
            if (ss >> type) {
                // si hay un entero al final, lo usamos
            }
            else {
                type = 1; // por defecto
            }

            Texture* t = (type == 2) ? textures[LOG2] : textures[LOG1];
			pos.setX(x); 
            pos.setY(y);
            addObject(
                new Log(this, t,
                    pos,
                    vx / FRAME_RATE,
                    leftSpan, rightSpan)
            );
            break;
        }

        case 'T': {
            float w = (float)textures[LOG2]->getFrameWidth();
            float h = (float)textures[LOG2]->getFrameHeight();
            int n, sink;

            if (!(ss >> x >> y >> vx >> n >> sink))
                throw FileFormatError(path, lineNum, "Invalid TurtleGroup line");
            
			pos.setX(x);
			pos.setY(y);
            addObject(
                new TurtleGroup(this, textures[LOG2],
                    pos,
                    w, h,
                    vx / FRAME_RATE,
                    leftSpan, rightSpan,
                    n, sink != 0)
            );
            break;
        }

        default:
            throw FileFormatError(path, lineNum, std::string("Unknown id: ") + id);
        }
    }
}


void Game::handleEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_EVENT_QUIT)
            exit = true;

        // Reiniciar con tecla R
        if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_R) {
            reset();
            return;
        }

        if (frog)
            frog->handleEvents(e);
    }
}

void Game::update()
{
    for (auto it = objects.begin(); it != objects.end(); ++it)
        (*it)->update();

    flushDeletions();
}

void Game::render() const
{
    SDL_RenderClear(renderer);

    textures[BACKGROUND]->render();

    for (auto* o : objects)
        o->render();

    SDL_RenderPresent(renderer);
}

Collision Game::checkCollision(const SDL_FRect& box) const
{
    for (auto* o : objects)
    {
        if (o == frog) continue;              // <<< evitar auto-colisión

        Collision c = o->checkCollision(box);
        if (c.type == Collision::Type::ENEMY ||
            c.type == Collision::Type::HOME)
            return c;
    }
    return {};
}

Collision Game::checkPlatform(const SDL_FRect& box) const
{
    for (auto* o : objects)
    {
        if (o == frog) continue;              // <<< igual aquí

        Collision c = o->checkCollision(box);
        if (c.type == Collision::Type::PLATFORM)
            return c;
    }
    return {};
}


void Game::flushDeletions()
{
    for (auto it : toDelete)
    {
        delete* it;
        objects.erase(it);
    }
    toDelete.clear();
}

void Game::run()
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
