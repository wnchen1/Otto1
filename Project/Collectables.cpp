#include "Collectables.h"


#include "Game.h"
#include "TextureManager.h"

Collectables::Collectables(SDL_Rect source, SDL_FRect destination, CollectableType item) : SpriteObject(source, destination)
{
	switch (item)
	{
	case key:
		TextureManager::Load("Assets/Images/key.png", "key");
			break;
	case key2:
		TextureManager::Load("Assets/Images/key.png", "key");
		break;
	}
	
}

void Collectables::Update(float deltaTime)
{
	
}

void Collectables::Render()
{
	SDL_RenderCopyExF(Game::GetInstance().GetRenderer(), TextureManager::GetTexture("key"), GetSourceTransform(), GetDestinationTransform(), 0.0, nullptr, SDL_FLIP_NONE);
}