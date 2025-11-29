#pragma once

#include "GameState.h"

class Label;
class Button;

/**
 * Menú de pausa.
 * Opciones:
 *  - Continuar (reanudar partida)
 *  - Reiniciar partida
 *  - Volver al menú principal
 *  - Salir del juego
 */
class PauseState : public GameState {
private:
    Label* pauseLabel = nullptr;  // fondo del menú
    Button* resumeButton = nullptr;  // CONTINUAR
    Button* restartButton = nullptr;  // REINICIAR
    Button* mainMenuButton = nullptr;  // VOLVER MENÚ
    Button* exitButton = nullptr;      // SALIR

public:
    PauseState(Game* game, GameStateMachine* gsm);
    ~PauseState() override;

    void update() override;
    void render() const override;
    void handleEvents(const SDL_Event& e) override;
};
