#pragma once
#ifndef __PLAYER__
#define __PLAYER__
#include "AnimatedSpriteObject.h"

class Player : public AnimatedSpriteObject
{
public:
	Player(SDL_Rect sourceTransform, SDL_FRect destinationTransform);
	~Player();

	// Life Cycle Methods
	virtual void Render() override;
	virtual void Update(float deltaTime) override;
	void SetX(float x) { m_destinationTransform.x = x; }
	void SetY(float y) { m_destinationTransform.y = y; }

private:
	enum class PlayerState { kIdle, kRunning, kAttacking };
	bool m_facingLeft;
	double speed = 0.5;
	PlayerState m_state;
};

#endif /* defined (__PLAYER__) */s