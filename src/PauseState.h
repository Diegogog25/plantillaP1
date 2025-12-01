#pragma once

#include "GameState.h"

class Label;
class Button;

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
