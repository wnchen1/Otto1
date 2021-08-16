#include "Boss.h"
#include "TextureManager.h"
#include "States.h"
#include "Game.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "CollisionManager.h"

Boss::Boss(SDL_Rect sourceTransform, SDL_FRect destinationTransform)
	: AnimatedSpriteObject(sourceTransform, destinationTransform),
	m_state(BossState::kRunning),
	m_facingLeft(false)
{
	TextureManager::Load("Assets/Images/mage_guardian-red.png", "red");

	SetAnimation(0.1, 0, 14, 0);

	SoundManager::SetSoundVolume(10);
	SoundManager::LoadSound("Assets/Sound/Effects/bowser_intro.wav", "Bintro");
	SoundManager::LoadSound("Assets/Sound/Effects/bowser_laugh.wav", "Blaugh");
	SoundManager::LoadSound("Assets/Sound/Effects/bowser_damage.wav", "Bdamage");
	SoundManager::LoadSound("Assets/Sound/Effects/bowser_dead.wav", "Bdead");
}

Boss::~Boss()
= default;

void Boss::Render()
{
	SDL_RenderCopyExF(Game::GetInstance().GetRenderer(), TextureManager::GetTexture("red"),
		&m_sourceTransform, &m_destinationTransform, 0.0, nullptr, (m_facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));
}

void Boss::Update(float deltaTime)
{
	switch (m_state)
	{
	case BossState::kRunning:
		Wander();
		SetAnimation(0.1, 0, 4, 0);
	break;
	case BossState::kAttacking:
	break;

	}
}

void Boss::Wander()
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
		m_destinationTransform.x -= 0.050f;
	}
	else if (roll == 2)
	{
		m_destinationTransform.x += 0.050f;
	}
	else if (roll == 3)
	{
		m_destinationTransform.y -= 0.050f;
	}
	if (roll == 4)
	{
		m_destinationTransform.y += 0.050f;
	}
}
