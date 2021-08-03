#pragma once
#ifndef __ENEMY__
#define __ENEMY__
#include "AnimatedSpriteObject.h"

class Enemy : public AnimatedSpriteObject
{
public:
	Enemy(SDL_Rect sourceTransform, SDL_FRect destinationTransform);
	~Enemy();

	// Life Cycle Methods
	virtual void Render() override;
	virtual void Update(float deltaTime) override;
	void SetX(float x) { m_destinationTransform.x = x; }
	void SetY(float y) { m_destinationTransform.y = y; }

private:
	enum class EnemyState { kIdle, kAttacking, kDying };
	bool m_facingLeft;
	double speed = 0.25;
	EnemyState m_state;
};

#endif /* defined (__ENEMY__) */s