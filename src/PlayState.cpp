#include "PlayState.h"
#include "SceneObject.h"
#include "Frog.h"
#include "collision.h"
#include "game.h"
#include "PauseState.h" // <-- añadir

#include <SDL3/SDL.h>

PlayState::PlayState(Game* g, GameStateMachine* gsm)
    : GameState(g, gsm)
{
    if (game) {
        // Prepara la partida con el mapa actualmente seleccionado en el menú
        game->reset();
        frog = game->getFrog();
    }
}

PlayState::~PlayState()
{
    // PlayState es propietario solo de sus propios SceneObject de escena
    // (por ahora no estamos usando sceneObjects, pero lo dejamos preparado)
    for (SceneObject* o : sceneObjects) {
        delete o;
    }
    sceneObjects.clear();
}

void PlayState::update()
{
    if (!game) return;

    // Lógica principal de Frogger (rana, coches, troncos, tortugas, avispas...)
    game->update();
}

void PlayState::render() const
{
    if (!game) return;

    // Fondo + escena del juego
    game->render();

    // Si en el futuro añadimos HUD / UI propio de este estado:
    GameState::render();
}

void PlayState::handleEvents(const SDL_Event& e)
{
    if (!game) return;

    // ESC -> abrir menú de pausa (apilar estado)
    if (e.type == SDL_EVENT_KEY_DOWN && !e.key.repeat && e.key.key == SDLK_ESCAPE) {
        game->pushState(new PauseState(game, nullptr)); // evitar conversión a GameStateMachine*
        return; // no propagar a la rana
    }

    // Tecla 0: pedir confirmación para reiniciar la partida
    if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_0) {
        int buttonId = -1;
        const SDL_MessageBoxButtonData buttons[] = {
            { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Cancelar" },
            { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Reiniciar" }
        };
        const SDL_MessageBoxData msgData = {
            SDL_MESSAGEBOX_WARNING,
            nullptr,                         // ventana padre (opcional)
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
                return; // no seguimos propagando este evento
            }
        }
    }

    Frog* current = game->getFrog();
    if (current != frog) frog = current;

    // La rana sigue recibiendo las teclas de movimiento
    if (frog) {
        frog->handleEvents(e);
    }

    // Y por último, cualquier listener registrado en este estado (HUD, etc.)
    GameState::handleEvents(e);
}

// --- Parte de "escena propia" de PlayState (de momento sin usar) ---

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
