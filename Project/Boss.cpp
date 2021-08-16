#include "Boss.h"
#include "TextureManager.h"
#include "Game.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "CollisionManager.h"

Boss::Boss(SDL_Rect sourceTransform, SDL_FRect destinationTransform, State* parent)
	: AnimatedSpriteObject(sourceTransform, destinationTransform),
	m_state(BossState::kRunning),
	m_facingLeft(true)
{
	TextureManager::Load("Assets/Images/boss", "boss");

	SetAnimation(0.1, 0, 12, 2);

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
	SDL_RenderCopyExF(Game::GetInstance().GetRenderer(), TextureManager::GetTexture("boss"),
		&m_sourceTransform, &m_destinationTransform, 0.0, nullptr, (m_facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));
}

void Boss::Update(float deltaTime)
{
	SDL_Rect enemyRect = { m_destinationTransform.x, m_destinationTransform.y, m_destinationTransform.w, m_destinationTransform.h };
	SDL_Rect playerRect = { statepointer->getPlayerPosition()->x, statepointer->getPlayerPosition()->y, statepointer->getPlayerPosition()->w, statepointer->getPlayerPosition()->h };

	switch (m_state)
	{
	case BossState::kRunning:
		if (CollisionManager::Distance(m_destinationTransform.x, player->GetDestinationTransform()->x,
			m_destinationTransform.y, player->GetDestinationTransform()->y) < 200)
		{
			m_state = BossState::kFollowing;
			SetAnimation(0.1, 0, 12, 2);
		}
		else if (CollisionManager::AABBCheck(enemyRect, playerRect))
		{
			m_state = BossState::kAttacking;
			SetAnimation(0.1, 0, 13, 0);
		}
		else Wander();
		break;

	case BossState::kAttacking:
		if (!CollisionManager::AABBCheck(enemyRect, playerRect))
		{
			m_state = BossState::kRunning;
			SetAnimation(0.1, 0, 12, 2);
		}
		break;
	
	case BossState::kFollowing:
		if (CollisionManager::AABBCheck(enemyRect, playerRect))
		{
			m_state = BossState::kAttacking;
			SetAnimation(0.1, 0, 13, 0);
		}
		else if (CollisionManager::Distance(m_destinationTransform.x, player->GetDestinationTransform()->x,
			m_destinationTransform.y, player->GetDestinationTransform()->y) > 200)
		{
			m_state = BossState::kRunning;
			SetAnimation(0.1, 0, 12, 2);
		}
		else Follow();
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
		m_facingLeft = true;
	}
	else if (roll == 2)
	{
		m_destinationTransform.x += 0.050f;
		m_facingLeft = false;
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

void Boss::Follow()
{
	if (m_destinationTransform.x < player->GetDestinationTransform()->x) //enemy is to the left of player
	{
		m_destinationTransform.x += 0.05;
		m_facingLeft = false;
	}
	if (m_destinationTransform.x > player->GetDestinationTransform()->x)//enemy is to the right of player
	{
		m_destinationTransform.x -= 0.05;
		m_facingLeft = true;
	}
	if (m_destinationTransform.y < player->GetDestinationTransform()->y) //enemy is above player
	{
		m_destinationTransform.y += 0.05;
	}
	if (m_destinationTransform.y > player->GetDestinationTransform()->y) //enemy is below player
	{
		m_destinationTransform.y -= 0.05;
	}
}