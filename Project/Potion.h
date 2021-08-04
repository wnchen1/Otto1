#pragma once
#include "Gameobject.h"
#include "Player.h"

class Potion : public GameObject
{
public:
	Potion(SDL_Rect sourceTransform, SDL_FRect destinationTransform);
	~Potion();
	void Use();
	void Update();
	void Render() override;
	bool hasPotion = false;
private:
	Player* pOtto;
	const SDL_Rect m_potionSrc = { 0, 0, 32, 32 };
	const SDL_FRect m_potionDst = { 0, 0, 32, 32 };
};