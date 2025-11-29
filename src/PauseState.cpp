#include "PauseState.h"
#include "Label.h"
#include "Button.h"
#include "game.h"
#include "gameStateMachine.h"
#include "MainMenuState.h"
#include "texture.h"
#include <SDL3/SDL.h>

PauseState::PauseState(Game* g, GameStateMachine* gsm)
    : GameState(g, gsm)
{
    if (!g) return;

    // Ya no usamos la textura MENUBACKGROUND; fondo será un rect negro semitransparente.
    Texture* continuarTx = g->getTexture(Game::CONTINUAR);
    Texture* reiniciarTx = g->getTexture(Game::REINICIAR);
    Texture* volverTx = g->getTexture(Game::VOLVER_MENU);
    Texture* salirTx = g->getTexture(Game::SALIR);

    SDL_FRect continuarRect{ 134.f, 86.f, continuarTx->getFrameWidth(), continuarTx->getFrameHeight() };
    SDL_FRect reiniciarRect{ 134.f, 182.f, reiniciarTx->getFrameWidth(), reiniciarTx->getFrameHeight() };
    SDL_FRect volverRect{ 84.f, 278.f, volverTx->getFrameWidth(), volverTx->getFrameHeight() };
    SDL_FRect salirRect{ 174.f, 374.f, salirTx->getFrameWidth(), salirTx->getFrameHeight() };

    resumeButton = new Button(g, continuarTx, continuarRect);
    restartButton = new Button(g, reiniciarTx, reiniciarRect);
    mainMenuButton = new Button(g, volverTx, volverRect);
    exitButton = new Button(g, salirTx, salirRect);

    // Añadimos solo los botones (pauseLabel se deja en nullptr para no dibujar textura de fondo)
    addObject(resumeButton);
    addObject(restartButton);
    addObject(mainMenuButton);
    addObject(exitButton);

    addEventListener(resumeButton);
    addEventListener(restartButton);
    addEventListener(mainMenuButton);
    addEventListener(exitButton);

    resumeButton->connect([this]() {
        if (game) game->popState();
    });
    restartButton->connect([this]() {
        if (game) game->reset();
        if (game) game->popState();
    });
    mainMenuButton->connect([this]() {
        if (!game) return;

        // Quita el PauseState (cima)
        game->popState();
        // Reemplaza el estado que quede (p.ej. PlayState) por el MainMenuState
        game->replaceState(std::make_shared<MainMenuState>(game, gameMachine));
    });
    exitButton->connect([this]() {
        // Salir del juego: enviar evento de salida a SDL
        SDL_Event quitEvent{};
        quitEvent.type = SDL_EVENT_QUIT;
        SDL_PushEvent(&quitEvent);
    });
}

PauseState::~PauseState() = default;

void PauseState::update()
{
    GameState::update();
}

void PauseState::render() const
{
    // Escena del juego debajo (congelada)
    if (game) game->render();

    // Overlay semitransparente negro
    if (game) {
        SDL_Renderer* r = game->getRenderer();
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(r, 0, 0, 0, 140); // alpha ~55%
        SDL_FRect rect{ 0.f, 0.f, (float)Game::WINDOW_WIDTH, (float)Game::WINDOW_HEIGHT };
        SDL_RenderFillRect(r, &rect);
    }

    // UI (botones) encima del overlay
    GameState::render();
}

void PauseState::handleEvents(const SDL_Event& e)
{
    if (e.type == SDL_EVENT_KEY_DOWN && !e.key.repeat && e.key.key == SDLK_ESCAPE) {
        if (game) game->popState();
        return;
    }
    GameState::handleEvents(e);
}