#pragma once
#ifndef __ENEMY__
#define __ENEMY__
#include "AnimatedSpriteObject.h"

class Enemy : public AnimatedSpriteObject
{
public:
	Enemy(SDL_Rect sourceTransform, SDL_FRect destinationTransform, int health, int attackPower);
	~Enemy();

	// Life Cycle Methods
	virtual void Render() override;
	virtual void Update(float deltaTime) override;
	void Attack();
	void Flee();
	void Seek();
	int takeDamage(int damage);
	int attack() { return attackValue; }
	int getHealth() { return health; }
private:
	enum class EnemyState { kIdle, kAttacking, kDying };
	bool m_facingLeft;
	bool m_attack;
	double speed = 0.25;
	int attackValue;
	int health;
	
	EnemyState m_state;
	
};

#endif /* defined (__ENEMY__) */s