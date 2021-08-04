#include "Potion.h"
#include "TextureManager.h"
#include "EventManager.h"
#include "Player.h"
#include "Game.h"

Potion::Potion(SDL_Rect sourceTransform, SDL_FRect destinationTransform)
{
	TextureManager::Load("Assets/Images/health.png", "potion");
	////////////Sounds
}

Potion::~Potion()
{
}

void Potion::Use()
{
	pOtto->playerLives++;
}

void Potion::Update()
{
	if (EventManager::KeyPressed(SDL_SCANCODE_P) && hasPotion == true)
	{
		Potion::Use();
	}
}

void Potion::Render()
{
	SDL_RenderCopyExF(Game::GetInstance().GetRenderer(),
		TextureManager::GetTexture("potion"), &transform, &m_destinationTransform, 0, 0, SDL_FLIP_NONE);
}
