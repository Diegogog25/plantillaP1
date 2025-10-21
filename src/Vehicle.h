#pragma once
#include "vector2D.h"
#include "texture.h"
#include "SDL3/SDL.h"
class Vehicle
{
private:
	Texture* texture;
	Point2D pos;
	Vector2D<> vel;
public:
	Vehicle(Texture* _texture, Point2D _pos, Vector2D<>_vel);
	~Vehicle();

	void render()const;
	void update();
	
};

