Collision Log::checkCollision(const SDL_FRect& rect) const {
    // Rect del tronco basado en el sprite
    SDL_FRect me{ pos.getX(), pos.getY(),
                  (float)texture->getFrameWidth(),
                  (float)texture->getFrameHeight() };

    // Recorta el rect del tronco para evitar los bordes transparentes/píxeles de holgura.
    // Así solo “cuenta” cuando la rana está visualmente encima, no al lado.
    const float insetX = me.w * 0.15f;      // recorte lateral (~15% por lado)
    const float insetTop = me.h * 0.10f;    // recorte superior
    const float insetBottom = me.h * 0.10f; // recorte inferior

    me.x += insetX;
    me.w = std::max(1.0f, me.w - 2.0f * insetX);
    me.y += insetTop;
    me.h = std::max(1.0f, me.h - insetTop - insetBottom);

    if (SDL_HasRectIntersectionFloat(&rect, &me)) {
        return Collision{ Collision::Type::PLATFORM, vel };
    }
    return {};
}