#pragma once
#include <stdexcept>
#include <string>
#include <SDL3/SDL.h>

struct GameError : std::logic_error {
    using std::logic_error::logic_error;
};

struct SDLError : GameError {
    explicit SDLError(const std::string& where)
        : GameError(where + ": " + SDL_GetError()) {
    }
};

struct FileNotFoundError : GameError {
    explicit FileNotFoundError(const std::string& path)
        : GameError("File not found: " + path) {
    }
};

struct FileFormatError : GameError {
    FileFormatError(const std::string& path, int line, const std::string& what)
        : GameError(path + ":" + std::to_string(line) + ": " + what) {
    }
};
