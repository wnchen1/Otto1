#pragma once
#ifndef __PLAYER__
#define __PLAYER__
#include "AnimatedSpriteObject.h"

class Player : public AnimatedSpriteObject
{
public:
	Player(SDL_Rect sourceTransform, SDL_FRect destinationTransform, int lives);
	~Player();

	// Life Cycle Methods
	virtual void Render() override;
	virtual void Update(float deltaTime) override;
	void SetX(float x) { m_destinationTransform.x = x; }
	void SetY(float y) { m_destinationTransform.y = y; }
	int getPlayerLive() { return playerLives; }
	int attack() { return attackValue; }
	int getHealth() { return health; }
	void GainLife() { playerLives++; if (playerLives>3) playerLives = 3;}
	void LoseLife() { playerLives--; if (playerLives<0) playerLives = 0;}

private:
	enum class PlayerState { kIdle, kRunning, kAttacking };
	bool m_facingLeft;
	float speed = 1;
	int attackValue = 15;
	int health = 100;
	int playerLives;
	PlayerState m_state;
};

#endif /* defined (__PLAYER__) */