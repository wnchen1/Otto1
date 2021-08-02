#include "Otto.h"
#include "TextureManager.h"
#include "States.h"

Player::Player() : m_currentAnimationState(PLAYER_IDLE_RIGHT)
{
	TextureManager::Instance().loadSpriteSheet(
		"../Assets/sprites/player.txt",
		"../Assets/sprites/player.png",
		"spritesheet");

	setSpriteSheet(TextureManager::Instance().getSpriteSheet("spritesheet"));

	// set frame width
	setWidth(53);

	// set frame height
	setHeight(58);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PLAYER);

	m_buildAnimations();
}

Player::~Player()
= default;

void Player::Render()
{
	//// alias for x and y
	//const auto x = getTransform()->position.x;
	//const auto y = getTransform()->position.y;

	//// draw the player according to animation state
	//switch (m_currentAnimationState)
	//{
	//case PLAYER_IDLE_RIGHT:
	//	TextureManager::Instance().playAnimation("spritesheet", getAnimation("idle"),
	//		x, y, 0.12f, 0, 255, true);
	//	break;
	//case PLAYER_IDLE_LEFT:
	//	TextureManager::Instance().playAnimation("spritesheet", getAnimation("idle"),
	//		x, y, 0.12f, 0, 255, true, SDL_FLIP_HORIZONTAL);
	//	break;
	//case PLAYER_RUN_RIGHT:
	//	TextureManager::Instance().playAnimation("spritesheet", getAnimation("run"),
	//		x, y, 0.25f, 0, 255, true);
	//	break;
	//case PLAYER_RUN_LEFT:
	//	TextureManager::Instance().playAnimation("spritesheet", getAnimation("run"),
	//		x, y, 0.25f, 0, 255, true, SDL_FLIP_HORIZONTAL);
	//	break;
	//case PLAYER_RUN_UP: //right for now
	//	TextureManager::Instance().playAnimation("spritesheet", getAnimation("run"),
	//		x, y, 0.25f, 0, 255, true);
	//	break;
	//case PLAYER_RUN_DOWN: //left for now
	//	TextureManager::Instance().playAnimation("spritesheet", getAnimation("run"),
	//		x, y, 0.25f, 0, 255, true, SDL_FLIP_HORIZONTAL);
	//	break;

	//default:
	//	break;
	//}

}

void Player::Update()
{
	//PlayScene* scene;
	//if (m_currentAnimationState == PLAYER_RUN_RIGHT /*&& scene->collision == false*/)
	//{
	//	getTransform()->position += glm::vec2(2.0f, 0.0f);
	//}
	///*else if (scene->collision == true) std::cout << "right side" << std::endl;*/

	//if (m_currentAnimationState == PLAYER_RUN_LEFT/* && scene->collision == false*/)
	//{
	//	getTransform()->position += glm::vec2(-2.0f, 0.0f);
	//}
	///*else if (scene->collision == true) std::cout << "left side" << std::endl;*/

	//if (m_currentAnimationState == PLAYER_RUN_DOWN /*&& scene->collision == false*/)
	//{
	//	getTransform()->position += glm::vec2(0.0f, 2.0f);
	//}
	///*else if (scene->collision == true) std::cout << "bottom" << std::endl;*/

	//if (m_currentAnimationState == PLAYER_RUN_UP /*&& scene->collision == false*/)
	//{
	//	getTransform()->position += glm::vec2(0.0f, -2.0f);
	//}
	///*else if (scene->collision == true) std::cout << "top" << std::endl;*/
}

void Player::clean()
{
}

void Player::setAnimationState(const PlayerAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void Player::m_buildAnimations()
{
	Animation idleAnimation = Animation();

	idleAnimation.name = "idle";
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("player-idle-0"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("player-idle-1"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("player-idle-2"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("player-idle-3"));

	setAnimation(idleAnimation);

	Animation runAnimation = Animation();

	runAnimation.name = "run";
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("player-run-0"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("player-run-1"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("player-run-2"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("player-run-3"));

	setAnimation(runAnimation);
}
