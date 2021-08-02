#include "Player.h"
#include "TextureManager.h"
#include "States.h"
#include "Game.h"
#include "EventManager.h"

Player::Player(SDL_Rect sourceTransform, SDL_FRect destinationTransform)
	: AnimatedSpriteObject(sourceTransform, destinationTransform),
	m_state(PlayerState::kIdle),
	m_facingLeft(false)
{
	TextureManager::Load("Assets/Images/otto.png", "otto");

	SetAnimation(0.1, 0, 13, 0);

//SOUNDS FOR PLAYER

	
}

Player::~Player()
= default;

void Player::Render()
{
	SDL_RenderCopyExF(Game::GetInstance().GetRenderer(), TextureManager::GetTexture("otto"),
		&m_sourceTransform, &m_destinationTransform, 0.0, nullptr, (m_facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));
}

void Player::Update(float deltaTime)
{
	switch (m_state)
	{
	case PlayerState::kIdle:
		// Transition to run.
		if (EventManager::KeyPressed(SDL_SCANCODE_W) || EventManager::KeyPressed(SDL_SCANCODE_A)
			|| EventManager::KeyPressed(SDL_SCANCODE_S) || EventManager::KeyPressed(SDL_SCANCODE_D))
		{
			m_state = PlayerState::kRunning;
			SetAnimation(0.1f, 0, 8, 64);
		}
		else if (EventManager::KeyPressed(SDL_SCANCODE_E))
		{
			m_state = PlayerState::kAttacking;
			SetAnimation(0.1f, 0, 10, 128);
		}
		break;

	////////////////Player Running////////////

	case PlayerState::kRunning:
		if (EventManager::KeyHeld(SDL_SCANCODE_W) /*&& m_destinationTransform.y > 0*/)
		{
			/*m_accelX = -s_kAccelerationX;*/

			m_destinationTransform.y -= .5;
			if (!m_facingLeft)
				m_facingLeft = true;
		}
		else if (EventManager::KeyHeld(SDL_SCANCODE_A) /*&& m_destinationTransform.x > 0*/)
		{
			/*m_accelX = -s_kAccelerationX;*/
			
			m_destinationTransform.x -= .5;
			if (!m_facingLeft)
				m_facingLeft = true;
		}
		else if (EventManager::KeyHeld(SDL_SCANCODE_S) /*&& m_destinationTransform.x
			< Game::kHeight - m_destinationTransform.y*/)
		{	
			m_destinationTransform.y += .5;
			if (m_facingLeft)
				m_facingLeft = false;
		}
		else if (EventManager::KeyHeld(SDL_SCANCODE_D) /*&& m_destinationTransform.x
			< Game::kWidth - m_destinationTransform.w*/)
		{
			m_destinationTransform.x += .5;
			if (m_facingLeft)
				m_facingLeft = false;
		}

		else if (!EventManager::KeyHeld(SDL_SCANCODE_W) && !EventManager::KeyHeld(SDL_SCANCODE_A)
			&& !EventManager::KeyHeld(SDL_SCANCODE_S) && !EventManager::KeyHeld(SDL_SCANCODE_D)
			&& !EventManager::KeyHeld(SDL_SCANCODE_E))
		{
			m_state = PlayerState::kIdle;
			SetAnimation(0.1f, 0, 13, 0);
		}
		else if (EventManager::KeyPressed(SDL_SCANCODE_E))
		{
			m_state = PlayerState::kAttacking;
			SetAnimation(0.1f, 0, 10, 128);
		}
		break;

		////////////////Player Attacking////////////

	case PlayerState::kAttacking:
		if (EventManager::KeyPressed(SDL_SCANCODE_W) || EventManager::KeyPressed(SDL_SCANCODE_A)
			|| EventManager::KeyPressed(SDL_SCANCODE_S) || EventManager::KeyPressed(SDL_SCANCODE_D))
		{
			m_state = PlayerState::kRunning;
			SetAnimation(0.1f, 0, 8, 64);
		}
		else if (!EventManager::KeyHeld(SDL_SCANCODE_W) && !EventManager::KeyHeld(SDL_SCANCODE_A)
			&& !EventManager::KeyHeld(SDL_SCANCODE_S) && !EventManager::KeyHeld(SDL_SCANCODE_D)
			&& !EventManager::KeyHeld(SDL_SCANCODE_E))
		{
			m_state = PlayerState::kIdle;
			SetAnimation(0.1f, 0, 13, 0);
		}
	}
	AnimatedSpriteObject::Update(deltaTime);
}



