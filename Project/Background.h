#pragma once
#include "GameObject.h"

class Background : public GameObject
{
public:
	Background(const char* textureKey);
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
private:
	const SDL_Rect m_BgSrc = {0, 0, 800, 609};
	const SDL_FRect m_BgDst = {0, 0, 800, 600};
};