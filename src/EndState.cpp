#include "EndState.h"
#include "Label.h"
#include "Button.h"
#include "game.h"
#include "gameStateMachine.h"

EndState::EndState(Game* g, GameStateMachine* gsm, bool /*playerWon*/)
    : GameState(g, gsm)
{
    Texture* tex = g->getTexture(Game::FROG); // placeholder

    SDL_FRect msgRect{ 120.f, 100.f, 200.f, 40.f };
    SDL_FRect mainRect{ 140.f, 180.f, 160.f, 40.f };
    SDL_FRect quitRect{ 140.f, 240.f, 160.f, 40.f };

    messageLabel = new Label(g, tex, msgRect);
    mainMenuButton = new Button(g, tex, mainRect);
    quitButton = new Button(g, tex, quitRect);

    addObject(messageLabel);
    addObject(mainMenuButton);
    addObject(quitButton);

    addEventListener(mainMenuButton);
    addEventListener(quitButton);

    mainMenuButton->connect([this]() {
        if (!gameMachine) return;
        // Sustituir por MainMenuState cuando lo integremos en la pila
        // Por ahora: simplemente vaciar y salir
        while (!gameMachine->empty()) {
            gameMachine->popState();
        }
        });

    quitButton->connect([this]() {
        if (!gameMachine) return;
        while (!gameMachine->empty()) {
            gameMachine->popState();
        }
        });
}

EndState::~EndState() = default;

void EndState::update()
{
    GameState::update();
}

void EndState::render() const
{
    GameState::render();
}

void EndState::handleEvents(const SDL_Event& e)
{
    GameState::handleEvents(e);
}
