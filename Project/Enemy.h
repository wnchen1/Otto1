#pragma once
#ifndef __ENEMY__
#define __ENEMY__
#include "AnimatedSpriteObject.h"
#include "States.h"
enum EnemyState { Idle, Attack, Run, Die};
class Enemy : public AnimatedSpriteObject
{
public:
	Enemy(SDL_Rect sourceTransform, SDL_FRect destinationTransform, int health, int attackPower, State* parent);
	~Enemy();

	// Life Cycle Methods
	virtual void Render() override;
	virtual void Update(float deltaTime) override;
	//void Wander();
	void SetX(float x) { m_destinationTransform.x = x; }
	void SetY(float y) { m_destinationTransform.y = y; }
	int attack() { return attackValue; }
	int getHealth() { return health; }
	float getSpeed() {return speed;}

	void Wander();


private:
	//enum class EnemyState { kIdle, kAttacking, kDying };
	bool m_facingLeft;
	bool m_attack;
	float speed = 0.25f;
	int attackValue;
	int health;
	SDL_FRect myPosition;
	EnemyState m_state;
	std::map<std::string, GameObject*> m_objects;

	int roll = 0;
	int eCounter = 0;
};

#endif /* defined (__ENEMY__) */s