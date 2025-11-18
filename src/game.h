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

class Game {
public:
    static constexpr int FRAME_RATE = 30;
    static constexpr int WINDOW_WIDTH = 448;
    static constexpr int WINDOW_HEIGHT = 484;
    static constexpr int RIVER_LOW = 180;

    enum TextureName { FROG = 0, BACKGROUND, CAR1, CAR2, CAR3, CAR4, CAR5, LOG1, LOG2, WASP, TURTLE, NUM_TEXTURES };

	using Anchor = std::list<SceneObject*>::iterator; // alias para iterador de lista polimórfica

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    std::array<Texture*, NUM_TEXTURES> textures{};
    std::list<SceneObject*> objects;      // lista polimórfica
    std::list<Anchor> toDelete;           // borrado 
    Frog* frog = nullptr;
    std::mt19937_64 rng{ std::random_device{}() };
    bool exit = false;

    // Nidos (coordenadas donde se colocan HomedFrog)
    static constexpr int firstH = 15;
    static constexpr int spaceH = 96;
    static constexpr int houseY = 22;
    int homedfrogs = 0;
    bool homedSpawned = false; // evita duplicar nidos

    // spawn de avispas
    static constexpr int minTime = 200, maxTime = 1000, maxSpawn = 1200;
    int waspTimer = 0;
    int nextWaspTime = 0; // NUEVO: siguiente instante de spawn
public:
    Game();
    ~Game();

    void run();
    void reset(); // reinicia partida
    void loadMap(const char* path);

    Texture* getTexture(TextureName n) const { return textures[n]; } 

    // colisiones: 2 pasadas (ENEMY/HOME y luego PLATFORM)
    Collision checkCollision(const SDL_FRect& box) const;
    Collision checkPlatform(const SDL_FRect& box) const;

    // lista polimórfica
    Anchor addObject(SceneObject* o) { objects.push_back(o); return std::prev(objects.end()); }
	void deleteAfter(Anchor it) { toDelete.push_back(it); } // marca para borrado
    void flushDeletions();

	int getRandomRange(int a, int b) { return std::uniform_int_distribution<int>(a, b)(rng); } // número aleatorio en rango [a,b] para spawn de avispas

private:
    void handleEvents();
    void update();
    void render() const;

    // Inicializa (si no existen ya) los nidos de HomedFrog
    void initHomedFrogs();
    void SpawnWasps();
};

#endif
