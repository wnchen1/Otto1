#pragma once
#include "SpriteObject.h"

class AnimatedSpriteObject : public SpriteObject 
{
public:
	virtual void Render() = 0;
	virtual void Update(float deltaTime) override;
protected:

	int m_currentSpriteIndex;	// Which sprite to display for animation.
	int m_startingSpriteIndex;	// The starting sprite index.
	int m_totalSprites;			// How many sprites total.

	float m_currentTime;		// Time counter.
	float m_frameRate;			// When to change to next sprite.

protected:
	AnimatedSpriteObject(const SDL_Rect source, const SDL_FRect destination);
	
	void SetAnimation(float frameRate, int startingSpriteIndex, int totalSprites, int sourceY = 0);
};

