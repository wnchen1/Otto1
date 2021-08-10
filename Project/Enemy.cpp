#include "Enemy.h"
#include "TextureManager.h"
#include "Game.h"
#include "EventManager.h"
#include "SoundManager.h"
#include <iostream>
#include "TiledLevel.h"

Enemy::Enemy(SDL_Rect sourceTransform, SDL_FRect destinationTransform, int hP, int attP, State* parent) :

	attackValue(attP),
	health(hP),
	AnimatedSpriteObject(sourceTransform, destinationTransform),
	m_state(EnemyState::Idle),
	m_facingLeft(false),
	m_attack(false)
{
	TextureManager::Load("Assets/Images/NightBorne.png", "nightb");

	SetAnimation(0.1, 0, 9, 0);
	
	statepointer = parent;
	//SOUNDS FOR PLAYER
	SoundManager::SetSoundVolume(10);
	SoundManager::LoadSound("Assets/Sound/Effects/creature_cry1.wav", "cry1");
}

Enemy::~Enemy()
= default;

void Enemy::Render()
{
	SDL_RenderCopyExF(Game::GetInstance().GetRenderer(), TextureManager::GetTexture("nightb"),
		&m_sourceTransform, &m_destinationTransform, 0.0, nullptr, (m_facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));
	SDL_Rect player = { statepointer->getPlayerPosition()->x, statepointer->getPlayerPosition()->y, statepointer->getPlayerPosition()->w, statepointer->getPlayerPosition()->h };
	SDL_Rect enemy = { m_destinationTransform.x - 20, m_destinationTransform.y - 20, m_destinationTransform.w + 20, m_destinationTransform.h + 20 };
	SDL_RenderDrawRect(Game::GetInstance().GetRenderer(), &player);
	SDL_RenderDrawRect(Game::GetInstance().GetRenderer(), &enemy);
}

void Enemy::Update(float deltaTime)
{
	//statepointer->getPlayerPosition()->x << std::endl; // m_destinationtransform;
	SDL_Rect player = { statepointer->getPlayerPosition()->x, statepointer->getPlayerPosition()->y, statepointer->getPlayerPosition()->w, statepointer->getPlayerPosition()->h };
	SDL_Rect enemy = { m_destinationTransform.x - 20, m_destinationTransform.y - 20, m_destinationTransform.w + 20, m_destinationTransform.h + 20 };
	Wander();
	switch (m_state)
	{
	case EnemyState::Idle:
		if (SDL_HasIntersection(&player, &enemy))
		{
			m_state = EnemyState::Attack;
			SetAnimation(0.1f, 0, 11, 160);
		}
		break;
	case EnemyState::Attack:
		if (!SDL_HasIntersection(&player, &enemy))
		{
			m_state = EnemyState::Idle;
			SetAnimation(0.1, 0, 9, 0);
			Wander();
		}
		break;
	}
	AnimatedSpriteObject::Update(deltaTime);

	int enemyLeft = m_destinationTransform.x;
	int enemyRight = m_destinationTransform.x + m_destinationTransform.w;
	int enemyTop = m_destinationTransform.y;
	int enemyBottom = m_destinationTransform.y + m_destinationTransform.h;

	for (unsigned int i = 0; i < static_cast<TiledLevel*>(m_objects["level1"])->GetObstacles().size(); i++)
	{
		SDL_FRect* obstacleColliderTransform = static_cast<TiledLevel*>(m_objects["level1"])->GetObstacles()[i]->GetDestinationTransform();

		float obstacleLeft = obstacleColliderTransform->x;
		float obstacleRight = obstacleColliderTransform->x + obstacleColliderTransform->w;
		float obstacleTop = obstacleColliderTransform->y;
		float obstacleBottom = obstacleColliderTransform->y + obstacleColliderTransform->h;

		bool xOverlap = enemyLeft < obstacleRight&& enemyRight > obstacleLeft;

		bool yOverlap = enemyTop < obstacleBottom&& enemyBottom > obstacleTop;

		float bottomCollision = obstacleBottom - m_destinationTransform.y;
		float topCollision = enemyBottom - obstacleColliderTransform->y;
		float leftCollision = enemyRight - obstacleColliderTransform->x;
		float rightCollision = obstacleRight - m_destinationTransform.x;

		if (xOverlap && yOverlap)
		{
			if (topCollision < bottomCollision && topCollision < leftCollision && topCollision < rightCollision)
			{
				SetY(m_destinationTransform.y - topCollision);
			}
			if (bottomCollision < topCollision && bottomCollision < leftCollision && bottomCollision < rightCollision)
			{
				SetY(m_destinationTransform.y + bottomCollision);
			}
			if (leftCollision < rightCollision && leftCollision < topCollision && leftCollision < bottomCollision)
			{
				SetX(m_destinationTransform.x - leftCollision);
			}
			if (rightCollision < leftCollision && rightCollision < topCollision && rightCollision < bottomCollision)
			{
				SetX(m_destinationTransform.x + rightCollision);
			}
		}
	}
}

void Enemy::Wander()
{ 
	eCounter++;
	if (eCounter > 300)
	{
		eCounter -= 300;
	}
	srand(time(NULL));
	if (eCounter == 1)
	{
		roll = rand() % 4 + 1;
	}

	if (roll == 1)
	{
		m_destinationTransform.x -= .1;
	}
	else if (roll == 2)
	{
		m_destinationTransform.x += .1;
	}
	else if (roll == 3)
	{
		m_destinationTransform.y -= .1;
	}
	if (roll == 4)
	{
		m_destinationTransform.y += .1;
	}
}


//void Enemy::Flee()
//{
//	
//}
//
void Enemy::Seek()
{
	
}



