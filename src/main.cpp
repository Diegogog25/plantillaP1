#include "game.h"
#include <iostream>
#include <string>

int main(int, char**)
{
    try {
        Game g;
        g.run();
    }
    catch (const char* msg) {
        SDL_Log("Error: %s", msg);
    }
    catch (const std::string& msg) {
        SDL_Log("Error: %s", msg.c_str());
    }
    return 0;
}
