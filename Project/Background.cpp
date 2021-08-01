#include "Background.h"
#include "TextureManager.h"
#include "Game.h"

Background::Background(const char* textureKey)
{
}

void Background::Update(float deltaTime)
{
}

void Background::Render()
{
	SDL_RenderCopyExF(Game::GetInstance().GetRenderer(),
		TextureManager::GetTexture("bg"), &m_BgSrc, &m_BgDst, 0, 0, SDL_FLIP_NONE);
}
