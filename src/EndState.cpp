#include "EndState.h"
#include "Label.h"
#include "Button.h"
#include "game.h"
#include "gameStateMachine.h"
#include "texture.h"
#include "MainMenuState.h"

EndState::EndState(Game* g, GameStateMachine* gsm, Texture* tex)
    : GameState(g, gsm)
{
    Texture* volverTx = g->getTexture(Game::VOLVER_MENU);
    Texture* salirTx = g->getTexture(Game::SALIR);

    // Rectángulos de los objetos
    SDL_FRect msgRect{ 130.f, 100.f, tex->getFrameWidth(),tex->getFrameHeight()};
    SDL_FRect volverRect{ 84.f, 200.f, volverTx->getFrameWidth(), volverTx->getFrameHeight()};
    SDL_FRect salirRect{ 174.f, 270.f, salirTx->getFrameWidth(), salirTx->getFrameHeight() };


    //Creamos botones
    messageLabel = new Label(g, tex, msgRect);
    mainMenuButton = new Button(g, volverTx, volverRect);
    quitButton = new Button(g, salirTx, salirRect);

    addObject(messageLabel);
    addObject(mainMenuButton);
    addObject(quitButton);

    addEventListener(mainMenuButton);
    addEventListener(quitButton);

    mainMenuButton->connect([this]() {
        if (!game || !gameMachine) return;

        // Vacio todos los estados
        while (!gameMachine->empty())
            gameMachine->popState();
        //Mete como unico estado el mainmenu
        gameMachine->pushState(new MainMenuState(game, gameMachine));
        });

    quitButton->connect([this]() {
        if (!game) return;
        game->exitTrue(); //Pone Exit = true para salir del bucle
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
