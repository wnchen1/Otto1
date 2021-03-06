#include "Collectables.h"


#include "Game.h"
#include "TextureManager.h"

Collectables::Collectables(SDL_Rect source, SDL_FRect destination, CollectableType item) : SpriteObject(source, destination)
{
	m_type = item;
	switch (m_type)
	{
	case key:
		TextureManager::Load("Assets/Images/key.png", "key");
			break;
	case key2:
		TextureManager::Load("Assets/Images/key.png", "key");
		break;
	case p1:
		TextureManager::Load("Assets/Images/health.png", "potion");
		break;
	/*case p2:
		TextureManager::Load("Assets/Images/health.png", "potion");
		break;
	case p3:
		TextureManager::Load("Assets/Images/health.png", "potion");
		break;
	case p4:
		TextureManager::Load("Assets/Images/health.png", "potion");
		break;*/
	}
	
}

void Collectables::Update(float deltaTime)
{
	
}

void Collectables::Render()
{
	switch (m_type)
	{
	case key:
		SDL_RenderCopyExF(Game::GetInstance().GetRenderer(), TextureManager::GetTexture("key"), GetSourceTransform(), GetDestinationTransform(), 0.0, nullptr, SDL_FLIP_NONE);
		break;
	case key2:
		SDL_RenderCopyExF(Game::GetInstance().GetRenderer(), TextureManager::GetTexture("key"), GetSourceTransform(), GetDestinationTransform(), 0.0, nullptr, SDL_FLIP_NONE);
		break;
	case p1:
		SDL_RenderCopyExF(Game::GetInstance().GetRenderer(), TextureManager::GetTexture("potion"),
			GetSourceTransform(), GetDestinationTransform(), 0.0, nullptr, SDL_FLIP_NONE);
	}
}
