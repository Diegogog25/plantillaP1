#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <array>
#include <istream>
#include <vector>
#include <random>
#include <algorithm>
#include <string>

#include "Vehicle.h"
#include "Log.h"
#include "Frog.h"
#include "HomedFrog.h"
#include "Wasp.h"
#include "texture.h"
#include "collision.h"

/**
 * Clase principal del juego.
 */
class Game
{
public:
    // Se actualiza el juego cada tantos milisegundos
    static constexpr int FRAME_RATE = 30;
    // Tamaño real de la ventana
    static constexpr int WINDOW_WIDTH = 448;
    static constexpr int WINDOW_HEIGHT = 484;
    // Extremo inferior del río
    static constexpr int RIVER_LOW = 210;

    enum TextureName
    {
        FROG = 0,
        BACKGROUND,
        CAR1,
        CAR2,
        CAR3,
        CAR4,
        CAR5,
        LOG1,
        LOG2,
        NUM_TEXTURES
    };

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    std::array<Texture*, NUM_TEXTURES> textures{};

    bool exit;

    // Objetos del juego (propiedad de Game: se crean con new y se liberan en ~Game)
    std::vector<Vehicle*>    vehicles;
    std::vector<Log*>        logs;
    std::vector<Wasp*>       wasps;
    std::vector<HomedFrog*>  homed;
    Frog* frog = nullptr;

    // RNG para avispas u otras mecánicas
    std::mt19937_64 rng{ std::random_device{}() };

    void render() const;
    void update();
    void handleEvents();

public:
    Game();
    ~Game();

    // Obtiene una textura por su nombre
    Texture* getTexture(TextureName name) const;

    // Ejecuta el bucle principal del juego
    void run();

    // Comprueba si hay algún objeto colocado en ese rectángulo (como pide el enunciado)
    Collision checkCollision(const SDL_FRect& rect) const;

    // Carga un mapa desde archivo (borra el estado current)
    void loadMap(const char* path);

    // RNG helper (como en el enunciado)
    int getRandomRange(int min, int max) {
        return std::uniform_int_distribution<int>(min, max)(rng);
    }
};

inline Texture* Game::getTexture(TextureName name) const
{
    return textures[name];
}

#endif // GAME_H
