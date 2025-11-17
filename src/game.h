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
#include "Wasp.h"
#include "HomedFrog.h"
#include "Frog.h"
#include "texture.h"
#include "collision.h"

class Game {
public:
    static constexpr int FRAME_RATE = 30;
    static constexpr int WINDOW_WIDTH = 448;
    static constexpr int WINDOW_HEIGHT = 484;
    static constexpr int RIVER_LOW = 210;

    enum TextureName { FROG_TEX = 0, BACKGROUND, CAR1, CAR2, CAR3, CAR4, CAR5, LOG1, LOG2, WASP, NUM_TEXTURES };

    using Anchor = std::list<SceneObject*>::iterator;

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    std::array<Texture*, NUM_TEXTURES> textures{};
    std::list<SceneObject*> objects;      // lista polimórfica
    std::list<Anchor> toDelete;           // borrado diferido
    Frog* frog = nullptr;
    std::mt19937_64 rng{ std::random_device{}() };
    bool exit = false;

    // Nidos (coordenadas donde se colocan HomedFrog)
    static constexpr int firstH = 20;
    static constexpr int spaceH = 96;
    static constexpr int houseY = 26;

    // spawn de avispas
    static constexpr int minTime = 200, maxTime = 1000, maxSpawn = 1200;
    int waspTimer = 0;

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
    void deleteAfter(Anchor it) { toDelete.push_back(it); }
    void flushDeletions();

    int getRandomRange(int a, int b) { return std::uniform_int_distribution<int>(a, b)(rng); }

private:
    void handleEvents();
    void update();
    void render() const;
};

#endif
