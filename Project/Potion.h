#pragma once
#include "Gameobject.h"

class Potion : public GameObject
{
public:
	Potion(SDL_Rect sourceTransform, SDL_FRect destinationTransform);
	~Potion();
	void Use();
	void Update();
	void Render() override;
	bool hasPotion;
private:
	Player* pOtto;
};