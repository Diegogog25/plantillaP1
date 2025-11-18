#pragma once
#include <stdexcept>
#include <string>
#include <SDL3/SDL.h>

struct GameError : std::logic_error { // error base del juego
    using std::logic_error::logic_error; 
};

struct SDLError : GameError { // error de SDL
    explicit SDLError(const std::string& where)
        : GameError(where + ": " + SDL_GetError()) {
    }
};

struct FileNotFoundError : GameError { // error de archivo no encontrado
    explicit FileNotFoundError(const std::string& path)
        : GameError("File not found: " + path) {
    }
};

struct FileFormatError : GameError { // error de formato de archivo
    FileFormatError(const std::string& path, int line, const std::string& what)
        : GameError(path + ":" + std::to_string(line) + ": " + what) {
    }
};
