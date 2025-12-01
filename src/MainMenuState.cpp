#include "MainMenuState.h"

#include "Label.h"
#include "Button.h"
#include "game.h"
#include "gameStateMachine.h"
#include "texture.h"

#include <filesystem>
#include <algorithm>
#include <cctype>
#include <iostream>

namespace fs = std::filesystem;

// Helper local: mapea nombre de mapa ("original") -> textura del texto (Game::ORIGINAL)
static Game::TextureName mapNameToTexture(const std::string& name) {
    std::string FileName = name;
    for (size_t i = 0; i < FileName.size(); ++i) {
        unsigned char uc = (unsigned char)FileName[i];   // paso seguro a unsigned char
        FileName[i] = (char)std::tolower(uc);            // tolower por carácter
    }

    if (FileName == "original")  return Game::ORIGINAL;
    if (FileName == "practica 1" || FileName == "práctica 1") return Game::PRACTICA1;
    if (FileName == "trivial")   return Game::TRIVIAL;
    if (FileName == "veloz")     return Game::VELOZ;
    if (FileName == "avispado")  return Game::AVISPADO;

    // Por si aparece algún mapa nuevo sin textura específica
    return Game::ORIGINAL;
}

MainMenuState::MainMenuState(Game* g, GameStateMachine* gsm)
    : GameState(g, gsm)
{
    loadMaps();

    Texture* titleTex = g->getTexture(Game::ELIGE_MAPA);
	Texture* leftTex = g->getTexture(Game::LEFT_ARROW);
	Texture* rightTex = g->getTexture(Game::RIGHT_ARROW);
	Texture* exitTex = g->getTexture(Game::SALIR);
    Texture* initialMapTex = mapNames.empty() ? g->getTexture(Game::ORIGINAL) : g->getTexture(mapNameToTexture(mapNames[currentIndex]));
        
    SDL_FRect titleRect{ 94.f,  200.f, titleTex->getFrameWidth(), titleTex->getFrameHeight()}; // "ELIGE UN MAPA"
    SDL_FRect mapRect{ 224.f - initialMapTex->getFrameWidth() / 2, 270.f, initialMapTex->getFrameWidth(), initialMapTex->getFrameHeight()}; // nombre del mapa
    SDL_FRect leftRect{ 80.f, 270.f,  leftTex->getFrameWidth(), leftTex->getFrameHeight()}; // flecha izquierda
    SDL_FRect rightRect{ 348.f, 270.f,  rightTex->getFrameWidth(), rightTex->getFrameHeight()}; // flecha derecha
    SDL_FRect exitRect{ 174.f, 350.f, exitTex->getFrameWidth(), exitTex->getFrameHeight()}; // SALIR

    titleLabel = new Label(g, titleTex, titleRect);
    leftArrow = new Label(g, leftTex, leftRect);
    rightArrow = new Label(g, rightTex, rightRect);
    exitButton = new Button(g, exitTex, exitRect);
    mapButton = new Button (g, initialMapTex, mapRect);
    // Registrar como objetos del estado
    addObject(titleLabel);
    addObject(mapButton);
    addObject(leftArrow);
    addObject(rightArrow);
    addObject(exitButton);

    // Botones escuchan eventos
    addEventListener(mapButton);
    addEventListener(exitButton);

    mapButton->connect([this]() {
        if (!game) return;

        if (!mapPaths.empty())
            game->setSelectedMap(mapPaths[currentIndex]);
        game->requestStartGame();
     });

    exitButton->connect([this]() {
        if (gameMachine) {
            while (!gameMachine->empty())
                gameMachine->popState();
        }
     });
    logCurrentMap();
}

void MainMenuState::loadMaps()
{
    mapPaths.clear();
    mapNames.clear();

    try {
        for (auto& entry : fs::directory_iterator("../assets/maps")) {
            if (!entry.is_regular_file()) continue;
            auto p = entry.path();
            mapPaths.push_back(p.string());
            mapNames.push_back(p.stem().string()); // "original", "trivial", etc.
        }
    }
    catch (const FileNotFoundError& e) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game Error", e.what(), nullptr);
    }

    if (mapPaths.empty()) {
        mapPaths.push_back("../assets/maps/turtles.txt");
        mapNames.push_back("original");
    }

    if (currentIndex >= (int)mapNames.size())
        currentIndex = 0;
}

void MainMenuState::updateMapLabel()
{
    if (!game || mapNames.empty()) return;

    Game::TextureName texName = mapNameToTexture(mapNames[currentIndex]);
    Texture* tex = game->getTexture(texName);
    mapButton->setTexture(tex);
}

void MainMenuState::logCurrentMap() const
{
    if (!mapNames.empty()) {
        std::cout << "Mapa seleccionado: " << mapNames[currentIndex] << std::endl;
    }
}

void MainMenuState::update()
{
    GameState::update();
}

void MainMenuState::render() const
{
    // Fondo: puedes usar el BACKGROUND del juego, o más tarde una textura específica
    if (game) {
        game->getTexture(Game::MENUBACKGROUND)->render();
    }

    // Pinta textos, flechas y botones
    GameState::render();
}

void MainMenuState::handleEvents(const SDL_Event& e)
{
    // Teclas para cambiar mapa y aceptar / salir
    if (e.type == SDL_EVENT_KEY_DOWN && !e.key.repeat) {
        switch (e.key.key) {
        case SDLK_LEFT:
            if (!mapNames.empty()) {
                currentIndex = (currentIndex - 1 + (int)mapNames.size()) % (int)mapNames.size();
                updateMapLabel();
                logCurrentMap();
            }
            break;
        case SDLK_RIGHT:
            if (!mapNames.empty()) {
                currentIndex = (currentIndex + 1) % (int)mapNames.size();
                updateMapLabel();
                logCurrentMap();
            }
            break;
        case SDLK_RETURN:
        case SDLK_KP_ENTER:
            if (game) {
                if (!mapPaths.empty())
                    game->setSelectedMap(mapPaths[currentIndex]);
                game->requestStartGame();
            }
            return;
        case SDLK_ESCAPE:
            if (gameMachine) {
                while (!gameMachine->empty())
                    gameMachine->popState();
            }
            return;
        default:
            break;
        }
    }

    // Ratón (botones CONTINUAR y SALIR)
    GameState::handleEvents(e);
}
