#pragma once

#include "GameState.h"

class Label;
class Button;

/**
 * Menú de fin de partida (ganar / perder).
 */
class EndState : public GameState {
private:
    Label* messageLabel = nullptr;
    Button* mainMenuButton = nullptr;
    Button* quitButton = nullptr;

public:
    // playerWon = true -> mensaje de victoria, si quieres usarlo más adelante
    EndState(Game* game, GameStateMachine* gsm, bool playerWon);
    ~EndState() override;

    void update() override;
    void render() const override;
    void handleEvents(const SDL_Event& e) override; // plural
};
