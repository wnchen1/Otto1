#pragma once
#include "AnimatedSpriteObject.h"

class PlatformPlayer : public AnimatedSpriteObject
{
	enum class PlayerState { kIdle, kRunning, kJumping };

	
	bool m_facingLeft;
	
	PlayerState m_state;

	static const float s_kAccelerationX;
	static const float s_kGravity;
	static const float s_kJumpForce;
	static const float s_kMaxVelocityX;
	static const float s_kMaxVelocityY;
	static const float s_kDragX;

public:
	PlatformPlayer(SDL_Rect sourceTransform, SDL_FRect destinationTransform);
	~PlatformPlayer();
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	void Jump();
	void Stop();
};

