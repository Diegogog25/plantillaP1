#include "game.h"
#include "Errors.h"
#include <string>

int main(int, char**) {
    try {
        Game().run();
    }
	catch (const GameError& e) { // errores específicos del juego
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game Error", e.what(), nullptr);
        return 1;
    }
	catch (const std::exception& e) { // otros errores estándar
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", e.what(), nullptr);
        return 1;
    }
    return 0;
}
