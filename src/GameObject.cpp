#include "GameObject.h"


GameObject::GameObject(Game* _game) { game = _game; } // constructor

GameObject::~GameObject() {} // destructor

void GameObject::update() {} // actualización vacía por defecto

void GameObject::render() const {} // renderizado vacío por defecto