#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <array>
#include <list>
#include <random>
#include <string>

#include "Errors.h"
#include "GameObject.h"
#include "SceneObject.h"
#include "Crosser.h"
#include "Platform.h"
#include "TurtleGroup.h"
#include "Vehicle.h"
#include "Log.h"
#include "HomedFrog.h"
#include "Frog.h"
#include "texture.h"
#include "collision.h"
#include "gameStateMachine.h"
#include "PlayState.h"

class Game : private GameStateMachine {
public:
    static constexpr int FRAME_RATE = 30;
    static constexpr int WINDOW_WIDTH = 448;
    static constexpr int WINDOW_HEIGHT = 484;
    static constexpr int RIVER_LOW = 180;

    enum TextureName {
        FROG = 0, BACKGROUND,
        CAR1, CAR2, CAR3, CAR4, CAR5,
        LOG1, LOG2,
        WASP, TURTLE,
        //--UI--
        MENUBACKGROUND,
        AVISPADO,
        CONTINUAR,
        ELIGE_MAPA,
        GAME_OVER,
        HAS_GANADO,
        LEFT_ARROW,
        ORIGINAL,
        PRACTICA1,
        REINICIAR,
        RIGHT_ARROW,
        SALIR,
        TRIVIAL,
        VELOZ,
        VOLVER_MENU,
        NUM_TEXTURES
    };

    using Anchor = std::list<SceneObject*>::iterator;

    // Hacemos visibles las operaciones de la pila de estados
    using GameStateMachine::pushState;
    using GameStateMachine::replaceState;
    using GameStateMachine::popState;

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    std::array<Texture*, NUM_TEXTURES> textures{};

    std::list<SceneObject*> objects;
    std::vector<HomedFrog*> homedFrogs{};
    std::list<Anchor>       toDelete;
    Frog* frog = nullptr;
    PlayState* playstate = nullptr;

    std::mt19937_64 rng{ std::random_device{}() };
    bool exit = false;

    // Nidos
    static constexpr int firstH = 15;
    static constexpr int spaceH = 96;
    static constexpr int houseY = 22;
    int  currentHomedFrogs = 0;
    int  occupied = 0;
    bool homedSpawned = false;

    // Avispas
    static constexpr int minTime = 75, maxTime = 200, maxSpawn = 1200;
    int waspTimer = 0;
    int nextWaspTime = 0;

    // Datos del menú principal
    bool        startRequested = false;
    std::string selectedMap = "./assets/maps/turtles.txt";

public:
    Game();
    ~Game();

    void run();
    void reset();                 // reinicia partida con el mapa seleccionado
    void loadMap(const char* path);

    // lógica de la partida (usada por PlayState)
    void update();
    void render() const;

    Texture* getTexture(TextureName n) const { return textures[n]; }

    // colisiones
    Collision checkCollision(const SDL_FRect& box) const;
    Collision checkPlatform(const SDL_FRect& box) const;

    // lista polimórfica P2
    Anchor addObject(SceneObject* o) { objects.push_back(o); return std::prev(objects.end()); }
    void   deleteAfter(Anchor it) { toDelete.push_back(it); }
    void   flushDeletions();

    int getRandomRange(int a, int b) {
        return std::uniform_int_distribution<int>(a, b)(rng);
    }

    SDL_Renderer* getRenderer() const { return renderer; }
    Frog* getFrog() const { return frog; }

    //Interfaz para el menu
    void setSelectedMap(const std::string& path) { selectedMap = path; }
    const std::string& getSelectedMap() const { return selectedMap; }

    void requestStartGame() { startRequested = true; }
    bool isStartRequested() const { return startRequested; }
    void clearStartRequest() { startRequested = false; }
    bool checkVictory();
    void startPlayState();
    void exitTrue() { exit = true;}
private:
    void initHomedFrogs();
    void SpawnWasps();
};

#endif
