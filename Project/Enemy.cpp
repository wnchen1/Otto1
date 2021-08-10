#include "Enemy.h"
#include "TextureManager.h"
#include "Game.h"
#include "EventManager.h"
#include "SoundManager.h"

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
	//SDL_RenderDrawRect(Game::GetInstance().GetRenderer(), &player);
	//SDL_RenderDrawRect(Game::GetInstance().GetRenderer(), &enemy);*/
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

		}
		break;
	}
	AnimatedSpriteObject::Update(deltaTime);

}


//void Enemy::Flee()
//{
//	
//}
//
//void Enemy::Seek()
//{
//	
//}
//


