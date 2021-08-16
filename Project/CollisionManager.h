#pragma once
#include <iostream>
#include <SDL.h>

class CollisionManager
{
public:
	static double Distance(double x1, double x2, double y1, double y2);

	static bool AABBCheck(SDL_Rect& object1, SDL_Rect& object2);

	static bool CircleCircleCheck(SDL_Point object1, SDL_Point object2, double r1, double r2);
	static bool CircleAABBCheck(SDL_Point object1, double r, SDL_Rect& object2);

	static bool LinePointCheck(SDL_Point object1_start, SDL_Point object1_end, SDL_Point object2);
};

typedef CollisionManager COMA;