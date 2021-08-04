#include "Potion.h"
#include "TextureManager.h"
#include "EventManager.h"
#include "Player.h"
#include "Game.h"

Potion::Potion(SDL_Rect sourceTransform, SDL_FRect destinationTransform, potionNumber num)
{
	pNum = num;
	switch (pNum)
	{
	case p1:
		TextureManager::Load("Assets/Images/health.png", "potion");
		break;
	case p2:
		TextureManager::Load("Assets/Images/health.png", "potion");
		break;
	case p3:
		TextureManager::Load("Assets/Images/health.png", "potion");
		break;
	case p4:
		TextureManager::Load("Assets/Images/health.png", "potion");
		break;
	}
}

void Potion::Use()
{
	pOtto->GainLife();
}

void Potion::Update(float deltaTime)
{
	if (EventManager::KeyPressed(SDL_SCANCODE_E) && hasPotion == true)
	{
		Potion::Use();
	}
}

void Potion::Render()
{
	switch (pNum)
	{
	case p1:
		SDL_RenderCopyExF(Game::GetInstance().GetRenderer(), TextureManager::GetTexture("potion"),
			&m_pSrc, &m_pDst, 0.0, nullptr, SDL_FLIP_NONE);
		break;
	case p2:
		SDL_RenderCopyExF(Game::GetInstance().GetRenderer(), TextureManager::GetTexture("potion"),
			&m_pSrc, &m_pDst, 0.0, nullptr, SDL_FLIP_NONE);
		break;
	case p3:
		SDL_RenderCopyExF(Game::GetInstance().GetRenderer(), TextureManager::GetTexture("potion"),
			&m_pSrc, &m_pDst, 0.0, nullptr, SDL_FLIP_NONE);
		break;
	case p4:
		SDL_RenderCopyExF(Game::GetInstance().GetRenderer(), TextureManager::GetTexture("potion"),
			&m_pSrc, &m_pDst, 0.0, nullptr, SDL_FLIP_NONE);
		break;
	}
}
