#include "Label.h"

Label::Label(Game* g, Texture* tex, const SDL_FRect& r)
    : GameObject(g), texture(tex), rect(r)
{
}

Label::~Label() = default;

void Label::render() const
{
    if (texture) {
        texture->render(rect);
    }
}

void Label::update()
{

}
