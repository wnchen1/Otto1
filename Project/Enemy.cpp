#include "Enemy.h"
#include "TextureManager.h"
#include "States.h"
#include "Game.h"
#include "EventManager.h"
#include "SoundManager.h"

Enemy::Enemy(SDL_Rect sourceTransform, SDL_FRect destinationTransform, int hP, int attP) :
	attackValue(attP),
	health(hP),
	AnimatedSpriteObject(sourceTransform, destinationTransform),
	m_state(EnemyState::kIdle),
	m_facingLeft(false),
	m_attack(false)
{
	TextureManager::Load("Assets/Images/NightBorne.png", "nightb");

	SetAnimation(0.1, 0, 9, 0);

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
}

void Enemy::Update(float deltaTime)
{
	switch (m_state)
	{
	case EnemyState::kIdle:
		AnimatedSpriteObject::Update(deltaTime);
	//	SetAnimation(0.1f, 0, 9, 0);
	}
	

}

