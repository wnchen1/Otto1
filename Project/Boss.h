#ifndef __BOSS__
#define __BOSS__
#include "AnimatedSpriteObject.h"
#include "PlatformPlayer.h"

class Boss : public AnimatedSpriteObject
{
public:
	Boss(SDL_Rect sourceTransform, SDL_FRect destinationTransform);
	~Boss();

	virtual void Render() override;
	virtual void Update(float deltaTime) override;
	void SetX(float x) { m_destinationTransform.x = x; }
	void SetY(float y) { m_destinationTransform.y = y; }
	void Wander();
	void Follow();

private:
	enum class BossState { kIdle, kAttacking, kRunning, kFollowing};
	bool m_facingLeft;
	double speed = 0.25;
	BossState m_state;
	PlatformPlayer* player;
	int eCounter = 0;
	int roll = 0;
};

#endif /* defined (__ENEMY__) */s