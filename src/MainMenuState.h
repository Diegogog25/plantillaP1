#pragma once

#include "GameState.h"
#include <vector>
#include <string>

class Label;
class Button;

/**
 * Menú principal:
 *  - Muestra "ELIGE UN MAPA"
 *  - Selector de mapa con flechas
 *  - Botón CONTINUAR (empezar partida)
 *  - Botón SALIR
 */
class MainMenuState : public GameState {
private:
    // Mapas detectados en ../assets/maps
    std::vector<std::string> mapPaths; // rutas completas
    std::vector<std::string> mapNames; // nombres sin extensión
    int currentIndex = 0;

    // UI
    Label* titleLabel = nullptr;  // ELIGE_MAPA
    Label* leftArrow = nullptr;  // LEFT_ARROW
    Label* rightArrow = nullptr;  // RIGHT_ARROW
    Button* mapButton = nullptr;  // ORIGINAL / PRACTICA1 / ...
    Button* exitButton = nullptr;  // SALIR

    void loadMaps();
    void updateMapLabel();
    void logCurrentMap() const;

public:
    MainMenuState(Game* game, GameStateMachine* gsm);
    ~MainMenuState() override = default;

    void update() override;
    void render() const override;
    void handleEvents(const SDL_Event& e) override; // plural
};
