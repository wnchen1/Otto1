//#pragma once
//#include "SpriteObject.h"
//#include "Player.h"
//
//enum potionNumber {p1, p2, p3, p4};
//class Potion : public SpriteObject
//{
//public:
//	Potion(SDL_Rect sourceTransform, SDL_FRect destinationTransform, potionNumber num);
//	~Potion() = default;
//	void Use();
//	void Update(float deltaTime) override;
//	void Render() override;
//	bool hasPotion = false;
//private:
//	Player* pOtto;
//	potionNumber pNum;
//	const SDL_Rect m_pSrc = { 0, 0, 32, 32 };
//	const SDL_FRect m_pDst = { 0, 0, 32, 32 };
//};