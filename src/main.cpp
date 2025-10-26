#include "game.h"
#include <iostream>
#include <string>

int main(int, char**)
{
    try {
        Game().run();
    }
    catch (const std::string& e) {
        std::cerr << "Error: " << e << std::endl;
        return 1;
    }
    catch (const char* e) {
        std::cerr << "Error: " << e << std::endl;
        return 1;
    }
    return 0;
}
