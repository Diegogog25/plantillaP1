#pragma once

#include "Label.h"
#include "EventHandler.h"
#include <functional>
#include <vector>

/**
 * Botón clicable:
 *  - se pinta como una Label
 *  - recibe eventos (EventHandler)
 *  - ejecuta callbacks al pulsarlo
 */
class Button : public Label, public EventHandler {
public:
    using Callback = std::function<void()>;

private:
    std::vector<Callback> callbacks;
    bool hovered = false;
    bool pressed = false;

    bool contains(float mx, float my) const {
        return mx >= rect.x && mx <= rect.x + rect.w &&
               my >= rect.y && my <= rect.y + rect.h;
    }

public:
    Button(Game* game, Texture* tex, const SDL_FRect& rect);
    ~Button() override = default;

    void connect(Callback cb);              // registrar callback
    void render() const override;           // resalta si hovered
    void update() override;                 // de momento vacío
    void handleEvent(const SDL_Event& e) override;

    bool isHovered() const { return hovered; }
};
