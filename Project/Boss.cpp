#include "Boss.h"
#include "TextureManager.h"
#include "States.h"
#include "Game.h"
#include "EventManager.h"
#include "SoundManager.h"

Boss::Boss(SDL_Rect sourceTransform, SDL_FRect destinationTransform)
	: AnimatedSpriteObject(sourceTransform, destinationTransform),
	m_state(BossState::kIdle),
	m_facingLeft(false)
{
	TextureManager::Load("Assets/Images/mage_guardian-blue.png", "blue");

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
	SDL_RenderCopyExF(Game::GetInstance().GetRenderer(), TextureManager::GetTexture("blue"),
		&m_sourceTransform, &m_destinationTransform, 0.0, nullptr, (m_facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));
}

void Boss::Update(float deltaTime)
{
	switch (m_state)
	{
	case BossState::kIdle:

		AnimatedSpriteObject::Update(deltaTime);
	}

}
