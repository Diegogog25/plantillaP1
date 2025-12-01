#include "PlayState.h"
#include "SceneObject.h"
#include "Frog.h"
#include "collision.h"
#include "game.h"
#include "PauseState.h"
#include "EndState.h"

#include <SDL3/SDL.h>

PlayState::PlayState(Game* g, GameStateMachine* gsm)
    : GameState(g, gsm)
{
    if (game) {
        // Crea la partida con el mapa seleccionado
        game->reset();
        frog = game->getFrog();
    }
}

PlayState::~PlayState()
{
    for (SceneObject* o : sceneObjects) {
        delete o;
    }
    sceneObjects.clear();
}

void PlayState::update()
{
    if (!game) return;

    game->update();
    Frog* frog = game->getFrog();

}

void PlayState::render() const
{
    if (!game) return;

    game->render();
    GameState::render();
}

void PlayState::handleEvents(const SDL_Event& e)
{
    if (!game) return;

    // ESC - abre menú de pausa 
    if (e.type == SDL_EVENT_KEY_DOWN && !e.key.repeat && e.key.key == SDLK_ESCAPE) {
        game->pushState(new PauseState(game, nullptr));
        return; 
    }

    // Tecla 0 - pide confirmación para reiniciar
    if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_0) {
        int buttonId = -1;
        const SDL_MessageBoxButtonData buttons[] = {
            { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Cancelar" },
            { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Reiniciar" }
        };
        const SDL_MessageBoxData msgData = {SDL_MESSAGEBOX_WARNING,
            nullptr,                        
            "Reiniciar partida",
            "¿Seguro que quieres reiniciar la partida?",
            SDL_arraysize(buttons),
            buttons,
            nullptr
        };
        if (SDL_ShowMessageBox(&msgData, &buttonId)) {
            if (buttonId == 1) {
                game->reset();
                frog = game->getFrog();
                return; 
            }
        }
    }

    Frog* current = game->getFrog();
    if (current != frog) frog = current;

    // La rana sigue recibiendo las teclas de movimiento
    if (frog) {
        frog->handleEvents(e);
    }

    GameState::handleEvents(e);
}


PlayState::SceneAnchor PlayState::addSceneObject(SceneObject* o)
{
    sceneObjects.push_back(o);
    return std::prev(sceneObjects.end());
}

void PlayState::removeSceneObject(SceneAnchor it)
{
    sceneObjects.erase(it);
}

Collision PlayState::checkCollision(const SDL_FRect& box) const
{
    Collision c;
    for (SceneObject* o : sceneObjects) {
        if (o == frog) continue;
        c = o->checkCollision(box);
        if (c.type != Collision::Type::NONE)
            return c;
    }
    return c;
}
//Comprueba si ha ganado o no y pasa la respectiva textura a endState
void PlayState::checkEnd(){
        Frog* f = game->getFrog();

    if (f && f->getLives() <= 0) {
        Texture* tex = game->getTexture(Game::GAME_OVER); 
        gameMachine->pushState(new EndState(game, gameMachine, tex));
        return;
    }

    if (game->checkVictory()) {
        Texture* tex = game->getTexture(Game::HAS_GANADO); 
        gameMachine->pushState(new EndState(game, gameMachine, tex));
        return;
    }
}