#include "game.h"
#include "Errors.h"
#include <string>

int main(int, char**) {
    try {
        Game().run();
    }
    catch (const GameError& e) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game Error", e.what(), nullptr);
        return 1;
    }
    catch (const std::exception& e) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", e.what(), nullptr);
        return 1;
    }
    return 0;
}
