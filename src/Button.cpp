#include "Button.h"
#include <SDL3/SDL.h>

Button::Button(Game* g, Texture* tex, const SDL_FRect& r)
    : Label(g, tex, r)
{
}

void Button::connect(Callback cb)
{
    callbacks.push_back(std::move(cb));
}

void Button::render() const
{
    if (!texture) return;

    SDL_FRect dst = rect;

    // Pequeño “zoom” si el ratón está encima
    if (hovered) {
        dst.x -= 2.f;
        dst.y -= 2.f;
        dst.w += 4.f;
        dst.h += 4.f;

        SDL_Color yellow{ 255, 255, 0, 255 };
        texture->render(dst, yellow);
    }
    else {
        texture->render(dst);
    }
    
}

void Button::update()
{
    // No hay lógica interna por ahora
}

void Button::handleEvent(const SDL_Event& e) {
    switch (e.type) {
    case SDL_EVENT_MOUSE_MOTION: {
        float mx = (float)e.motion.x;
        float my = (float)e.motion.y;
        hovered = contains(mx, my);
        break;
    }
    case SDL_EVENT_MOUSE_BUTTON_DOWN: {
        if (e.button.button == SDL_BUTTON_LEFT) {
            float mx = (float)e.button.x;
            float my = (float)e.button.y;
            pressed = contains(mx, my);
        }
        break;
    }
    case SDL_EVENT_MOUSE_BUTTON_UP: {
        if (e.button.button == SDL_BUTTON_LEFT) {
            float mx = (float)e.button.x;
            float my = (float)e.button.y;
            bool upInside = contains(mx, my);
            if (pressed && upInside) {
                for (Callback& cb : callbacks) cb();
            }
            pressed = false;
        }
        break;
    }
    case SDL_EVENT_KEY_DOWN: {
        if (!e.key.repeat && (e.key.key == SDLK_RETURN || e.key.key == SDLK_SPACE)) {
            if (hovered) {
                for (Callback& cb : callbacks) cb();
            }
        }
        break;
    }
    default: break;
    }
}
