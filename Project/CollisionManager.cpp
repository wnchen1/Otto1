#include "CollisionManager.h"

double CollisionManager::Distance(double x1, double x2, double y1, double y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

bool CollisionManager::AABBCheck(SDL_Rect& object1, SDL_Rect& object2)
{
	return SDL_HasIntersection(&object1, &object2);
}

bool CollisionManager::CircleCircleCheck(SDL_Point object1, SDL_Point object2, double r1, double r2)
{
	return (Distance(object1.x, object2.x, object1.y, object2.y) < (r1+r2));
}

bool CollisionManager::CircleAABBCheck(SDL_Point object1, double r, SDL_Rect& object2)
{
	double x1 = object1.x;
	double y1 = object1.y;
	if (object1.x < object2.x) // Circle center, rect left edge.
		x1 = object2.x;  
	else if (object1.x > object2.x + object2.w)
		x1 = object2.x + object2.w;
	if (object1.y < object2.y)
		y1 = object2.y;
	else if (object1.y > object2.y + object2.h)
		y1 = object2.y + object2.h;
	// Finish with Py-theorem...
	// How can I use the circleCircleCheck() method instead of the next four lines? Hint: one of the radii.
	double distance = Distance(x1, object1.x, y1, object1.y);
	if (distance < r) 
		return true;
	return false;
}

bool CollisionManager::LinePointCheck(SDL_Point object1_start, SDL_Point object1_end, SDL_Point object2)
{
	double distToStart = Distance(object1_start.x, object2.x, object1_start.y, object2.y);
	double distToEnd = Distance(object1_end.x, object2.x, object1_end.y, object2.y);

	double lineLength = Distance(object1_start.x, object1_end.x, object1_start.y, object1_end.y);

	double buffer = 0.2; // Extra distance since line thickness is one pixel.
	
	if (distToStart + distToEnd <= lineLength + buffer)
		return true;
	return false;
}
