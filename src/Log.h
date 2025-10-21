#pragma once
#include "vector2D.h"
#include "texture.h"
#include "SDL3/SDL.h"
class Log
{
private:
		Texture* texture;
		Point2D pos;
		Vector2D<> vel;
public:
		Log(Texture* _texture, Point2D _pos, Vector2D<>_vel);
		~Log();

		void render()const;
		void update();
};

