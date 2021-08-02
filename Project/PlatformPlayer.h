#pragma once
#include "AnimatedSpriteObject.h"

class PlatformPlayer : public AnimatedSpriteObject
{
	enum class PlayerState { kIdle, kRunning, kJumping };

	bool m_grounded;
	bool m_facingLeft;
	float m_accelX;
	float m_accelY;
	float m_velX;
	float m_velY;
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

	void SetAccelX(float a) { m_accelX = a; }
	void SetAccelY(float a) { m_accelY = a; }
	bool IsGrounded() { return m_grounded; }
	void SetGrounded(bool g) { m_grounded = g; }
	float GetVelX() { return m_velX; }
	float GetVelY() { return m_velY; }
	void SetX(float x) { m_destinationTransform.x = x; }
	void SetY(float y) { m_destinationTransform.y = y; }
};

