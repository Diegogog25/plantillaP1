#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

struct SDL_Texture;

class Texture
{
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
    int width = 0;
    int height = 0;
    int nrows = 1;
    int ncolumns = 1;
    int frameWidth = 0;
    int frameHeight = 0;

    SDL_FRect getFrameRect(int row, int col) const;

public:
    Texture(SDL_Renderer* renderer, const char* filename, int rows = 1, int columns = 1);
    Texture(SDL_Renderer* renderer, SDL_Texture* texture, int rows = 1, int columns = 1);
    ~Texture();

    Texture(Texture&&) noexcept;
    Texture& operator=(Texture&&) noexcept;

    int getFrameWidth() const { return frameWidth; }
    int getFrameHeight() const { return frameHeight; }
    int getNumRows() const { return nrows; }
    int getNumColumns() const { return ncolumns; }

    void render() const;
    void render(const SDL_FRect& rect) const;
    void renderRect(const SDL_FRect& source) const;
    void render(const SDL_FRect& rect, double angle, const SDL_FPoint* center = nullptr, SDL_FlipMode flip = SDL_FLIP_NONE) const;

    void renderFrame(const SDL_FRect& rect, int row, int col) const;
    void renderFrame(const SDL_FRect& rect, int row, int col, SDL_FlipMode flip) const;
    void renderFrame(const SDL_FRect& rect, int row, int col, double angle, const SDL_FPoint* center = nullptr, SDL_FlipMode flip = SDL_FLIP_NONE) const;

    void render(const SDL_FRect& rect, SDL_Color color) const;
};
