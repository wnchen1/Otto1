#pragma once
#ifndef __PLAYER__
#define __PLAYER__
#include "AnimatedSpriteObject.h"
#include "PlayerAnimationState.h"
#include "Animation.h"

class Player : public AnimatedSpriteObject
{
public:
	Player();
	~Player();

	// Life Cycle Methods
	virtual void Render() override;
	virtual void Update() override;
	virtual void clean() override;

	// setters
	void setAnimationState(PlayerAnimationState new_state);

private:
	void m_buildAnimations();

	PlayerAnimationState m_currentAnimationState;
};

#endif /* defined (__PLAYER__) */