#pragma once

#include "GameState.h"
#include "texture.h"

class Label;
class Button;

class EndState : public GameState {
private:
    Label* messageLabel = nullptr;
    Button* mainMenuButton = nullptr;
    Button* quitButton = nullptr;


public:

    EndState(Game* game, GameStateMachine* gsm,Texture* tex);
    ~EndState() override;

    void update() override;
    void render() const override;
    void handleEvents(const SDL_Event& e) override; 
};
