#include "EndState.h"
#include "Label.h"
#include "Button.h"
#include "game.h"
#include "gameStateMachine.h"
#include "Texture.h"

EndState::EndState(Game* g, GameStateMachine* gsm, Texture* tex)
    : GameState(g, gsm)
{
    // Rectángulos de los objetos
    SDL_FRect msgRect{ 120.f, 100.f, 200.f, 40.f };
    SDL_FRect mainRect{ 140.f, 180.f, 160.f, 40.f };
    SDL_FRect quitRect{ 140.f, 240.f, 160.f, 40.f };

    Texture* volverTx = g->getTexture(Game::VOLVER_MENU);
    Texture* salirTx = g->getTexture(Game::SALIR);
    // Crear los objetos del menú
    messageLabel = new Label(g, tex, msgRect);
    mainMenuButton = new Button(g, volverTx, mainRect);
    quitButton = new Button(g, salirTx, quitRect);

    // Añadirlos al estado
    addObject(messageLabel);
    addObject(mainMenuButton);
    addObject(quitButton);

    // Registrar listeners
    addEventListener(mainMenuButton);
    addEventListener(quitButton);

    // Conectar acciones de los botones
    mainMenuButton->connect([this]() {
        if (!gameMachine) return;
        while (!gameMachine->empty())
            gameMachine->popState();
        });

    quitButton->connect([this]() {
        if (!game) return;
        game->exitTrue();  // tu bool 'exit' en Game
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
