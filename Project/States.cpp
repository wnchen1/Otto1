#include <iostream>

#include "States.h"
#include "Game.h"
#include "StateManager.h"
#include "TextureManager.h"
#include "EventManager.h"
#include "TiledLevel.h"
#include "PlatformPlayer.h"
#include "SoundManager.h"
#include "PlayButton.h"
#include "ExitButton.h"
#include "Background.h"
#include "Player.h"
#include "Enemy.h"

////////////////////////////////////////////
// Begin TitleState ////////////////////////
void TitleState::Enter()
{
	std::cout << "Entering TitleState..." << std::endl;
	SoundManager::LoadMusic("Assets/Sound/Music/Blood Lord - The Invitation.mp3", "bgm1");
	SoundManager::SetMusicVolume(20);
	SoundManager::PlayMusic("bgm1");

	TextureManager::Load("Assets/Images/Buttons/play.png", "play");
	TextureManager::Load("Assets/Images/Buttons/exit.png", "exit");
	TextureManager::Load("Assets/Images/bg.png", "bg");

	int buttonWidth = 400;
	int buttonHeight = 100;
	float buttonX = Game::GetInstance().kWidth / 2 - buttonWidth / 2.0f;
	float buttonY = Game::GetInstance().kHeight / 2 - buttonHeight / 2.0f;
	
	m_objects.emplace("bg", new Background("bg"));
	m_objects.emplace("play", new PlayButton({ 0, 0, buttonWidth, buttonHeight }, { buttonX, buttonY, (float)buttonWidth, (float)buttonHeight }, "play"));
	m_objects.emplace("exit", new ExitButton({ 0, 0, buttonWidth, buttonHeight }, { 200, 400, (float)buttonWidth, (float)buttonHeight }, "exit"));
}

void TitleState::Update(float deltaTime)
{
	for (auto const& i : m_objects)
	{
		i.second->Update(deltaTime);
		if (StateManager::StateChanging()) 
			return;
	}
}

void TitleState::Render()
{
	//SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 255, 255);
	SDL_RenderClear(Game::GetInstance().GetRenderer());

	for (auto const& i : m_objects)
		i.second->Render();
}

void TitleState::Exit()
{
	std::cout << "Exiting TitleState..." << std::endl;
	SoundManager::StopMusic();
	SoundManager::UnloadMusic("bgm1");

	TextureManager::Unload("bg");
	TextureManager::Unload("play");
	TextureManager::Unload("exit");
	for (auto& i : m_objects)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_objects.clear();
}
// End TitleState

////////////////////////////////////////////
// Begin GameState (Level 1)////////////////
void GameState::Enter() // Used for initialization.
{
	std::cout << "Entering GameState..." << std::endl;

	m_RectangleTransform.x = Game::kWidth / 2;
	m_RectangleTransform.y = Game::kHeight / 2;
	m_RectangleTransform.w = 60;
	m_RectangleTransform.h = 100;

	TextureManager::Load("Assets/Images/Tiles.png", "tiles");

	TextureManager::Load("Assets/Images/3.png", "3h");
	TextureManager::Load("Assets/Images/2.png", "2h");
	TextureManager::Load("Assets/Images/1.png", "1h");

	TextureManager::Load("Assets/Images/key.png", "key");
	TextureManager::Load("Assets/Images/health.png", "health");

	m_objects.emplace("level", new TiledLevel(19, 25, 32, 32, "Assets/Data/Tiledata.txt", "Assets/Data/Level1.txt", "tiles"));
	m_objects.emplace("otto", new Player({ 0, 0, 64, 64 }, { 400, 200, 32, 32 }));

	SoundManager::LoadMusic("Assets/Sound/Music/Blood Lord - A Long Journey.mp3", "bgm2");
	SoundManager::SetMusicVolume(10);
	SoundManager::PlayMusic("bgm2");
}

void GameState::Update(float deltaTime)
{
	if (EventManager::KeyPressed(SDL_SCANCODE_X))
	{
		StateManager::ChangeState(new TitleState()); // Change to new TitleState
	}
	else if (EventManager::KeyPressed(SDL_SCANCODE_P))
	{
		StateManager::PushState(new PauseState()); // Add new PauseState
	}
	else
	{
		for (auto const& i : m_objects)
		{
			i.second->Update(deltaTime);
		}
		
		 //Check collision.
		SDL_FRect* playerColliderTransform = m_objects["otto"]->GetDestinationTransform(); // Copies address of player m_dst.

		float playerLeft = playerColliderTransform->x;
		float playerRight = playerColliderTransform->x + playerColliderTransform->w;
		float playerTop = playerColliderTransform->y;
		float playerBottom = playerColliderTransform->y + playerColliderTransform->h;

		Player* pPlayer = static_cast<Player*>(m_objects["otto"]);

		for (unsigned int i = 0; i < static_cast<TiledLevel*>(m_objects["level"])->GetObstacles().size(); i++)
		{
			SDL_FRect* obstacleColliderTransform = static_cast<TiledLevel*>(m_objects["level"])->GetObstacles()[i]->GetDestinationTransform();

			float obstacleLeft = obstacleColliderTransform->x;
			float obstacleRight = obstacleColliderTransform->x + obstacleColliderTransform->w;
			float obstacleTop = obstacleColliderTransform->y;
			float obstacleBottom = obstacleColliderTransform->y + obstacleColliderTransform->h;

			 //Check if they overlap horizontally
			 //by comparing right vs. left and left vs. right.
			bool xOverlap = playerLeft < obstacleRight && playerRight > obstacleLeft;

			 //Check if they also overlap vertically
			 //by comparing top vs. bottom and bottom vs. top.
			bool yOverlap = playerTop < obstacleBottom && playerBottom > obstacleTop;

			 //Used to determine which direction the collision came from
			float bottomCollision = obstacleBottom - playerColliderTransform->y;
			float topCollision = playerBottom - obstacleColliderTransform->y;
			float leftCollision = playerRight - obstacleColliderTransform->x;
			float rightCollision = obstacleRight - playerColliderTransform->x;

			 //If they overlap both horizontally and vertically,
			 //then they truly overlap.
			if (xOverlap && yOverlap)
			{
				if (topCollision < bottomCollision && topCollision < leftCollision && topCollision < rightCollision)
				{
					pPlayer->SetY(playerColliderTransform->y - topCollision);
				}
				if (bottomCollision < topCollision && bottomCollision < leftCollision && bottomCollision < rightCollision)
				{
					pPlayer->SetY(playerColliderTransform->y + bottomCollision);
				}
				if (leftCollision < rightCollision && leftCollision < topCollision && leftCollision < bottomCollision)
				{
					pPlayer->SetX(playerColliderTransform->x - leftCollision);
				}
				if (rightCollision < leftCollision && rightCollision < topCollision && rightCollision < bottomCollision)
				{
					pPlayer->SetX(playerColliderTransform->x + rightCollision);
				}
			}
		}


	}
}

void GameState::Render()
{
	SDL_Renderer* pRenderer = Game::GetInstance().GetRenderer();

	SDL_SetRenderDrawColor(pRenderer, 61, 37, 59, 225);
	SDL_RenderClear(pRenderer);

	for (auto const& i : m_objects)
		i.second->Render();

	///////////LIVES///////////////
	SDL_FRect* playerPos = m_objects["otto"]->GetDestinationTransform();
	const SDL_Rect m_h1Src = { 0, 0, 25, 25 };
	const SDL_Rect m_h2Src = { 0, 0, 50, 25 };
	const SDL_Rect m_h3Src = { 0, 0, 75, 25 };
	const SDL_FRect m_h1Dst = { playerPos->x , playerPos->y - 15, 25, 25 };
	const SDL_FRect m_h2Dst = { playerPos->x - 10 , playerPos->y - 15, 50, 25 };
	const SDL_FRect m_h3Dst = { playerPos->x - 22, playerPos->y - 15, 75, 25 };
	if (lives == 3)
	{
		SDL_RenderCopyExF(Game::GetInstance().GetRenderer(),
			TextureManager::GetTexture("3h"), &m_h3Src, &m_h3Dst, 0, 0, SDL_FLIP_NONE);
	}
	else if (lives == 2)
	{
		SDL_RenderCopyExF(Game::GetInstance().GetRenderer(),
			TextureManager::GetTexture("2h"), &m_h2Src, &m_h2Dst, 0, 0, SDL_FLIP_NONE);
	}
	else if (lives == 1)
	{
		SDL_RenderCopyExF(Game::GetInstance().GetRenderer(),
			TextureManager::GetTexture("1h"), &m_h1Src, &m_h1Dst, 0, 0, SDL_FLIP_NONE);
	}
	//////////////////////////////////
}

void GameState::Exit()
{
	std::cout << "Exiting GameState..." << std::endl;

	TextureManager::Unload("tiles");
	TextureManager::Unload("player");
	for (auto& i : m_objects)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_objects.clear();

	SoundManager::StopMusic();
	SoundManager::UnloadMusic("bgm2");
}

void GameState::Pause()
{
	std::cout << "Pauseing GameState..." << std::endl;
	SoundManager::PauseMusic();
}

void GameState::Resume()
{
	std::cout << "Resuming GameState..." << std::endl;
	SoundManager::ResumeMusic();
}
// End GameState

////////////////////////////////////////////
// Begin PauseState ////////////////////////
void PauseState::Enter()
{
	std::cout << "Entering PauseState..." << std::endl;
}

void PauseState::Update(float deltaTime)
{
	if (EventManager::KeyPressed(SDL_SCANCODE_R))
	{
		StateManager::PopState();
	}
}

void PauseState::Render()
{
	// First render the GameState
	StateManager::GetStates().front()->Render();
	// Now render rest of PauseState
	SDL_SetRenderDrawBlendMode(Game::GetInstance().GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 128, 128, 128, 128);
	SDL_Rect rect = { 1, 1, 800, 608 };
	SDL_RenderFillRect(Game::GetInstance().GetRenderer(), &rect);
}

void PauseState::Exit()
{
	std::cout << "Exiting PauseState..." << std::endl;
}
// End PauseState

////////////////////////////////////////////
// Begin GameState1 (Level 2)///////////////
void GameState2::Enter() // Used for initialization.
{
	std::cout << "Entering GameState..." << std::endl;

	TextureManager::Load("Assets/Images/Tiles.png", "tiles");
	TextureManager::Load("Assets/Images/Player.png", "player");


	m_objects.emplace("level", new TiledLevel(19, 25, 32, 32, "Assets/Data/Tiledata.txt", "Assets/Data/Level1.txt", "tiles"));
	m_objects.emplace("items", new TiledLevel(19, 25, 32, 32, "Assets/Data/Tiledata.txt", "Assets/Data/Level1 Items.txt", "tiles"));
	m_objects.emplace("otto", new Player({ 0, 0, 64, 64 }, { 400, 200, 32, 32 }));

	SoundManager::LoadMusic("Assets/Sound/Music/Blood Lord - At the Gates.mp3", "bgm3");
	SoundManager::SetMusicVolume(10);
	SoundManager::PlayMusic("bgm3");
}

void GameState2::Update(float deltaTime)
{
	if (EventManager::KeyPressed(SDL_SCANCODE_X))
	{
		StateManager::ChangeState(new TitleState()); // Change to new TitleState
	}
	else if (EventManager::KeyPressed(SDL_SCANCODE_P))
	{
		StateManager::PushState(new PauseState()); // Add new PauseState
	}
	else
	{
		for (auto const& i : m_objects)
		{
			i.second->Update(deltaTime);
		}

		//Check collision.
		SDL_FRect* playerColliderTransform = m_objects["otto"]->GetDestinationTransform(); // Copies address of player m_dst.

		float playerLeft = playerColliderTransform->x;
		float playerRight = playerColliderTransform->x + playerColliderTransform->w;
		float playerTop = playerColliderTransform->y;
		float playerBottom = playerColliderTransform->y + playerColliderTransform->h;

		Player* pPlayer = static_cast<Player*>(m_objects["otto"]);

		for (unsigned int i = 0; i < static_cast<TiledLevel*>(m_objects["level"])->GetObstacles().size(); i++)
		{
			SDL_FRect* obstacleColliderTransform = static_cast<TiledLevel*>(m_objects["level"])->GetObstacles()[i]->GetDestinationTransform();

			float obstacleLeft = obstacleColliderTransform->x;
			float obstacleRight = obstacleColliderTransform->x + obstacleColliderTransform->w;
			float obstacleTop = obstacleColliderTransform->y;
			float obstacleBottom = obstacleColliderTransform->y + obstacleColliderTransform->h;

			//Check if they overlap horizontally
			//by comparing right vs. left and left vs. right.
			bool xOverlap = playerLeft < obstacleRight&& playerRight > obstacleLeft;

			//Check if they also overlap vertically
			//by comparing top vs. bottom and bottom vs. top.
			bool yOverlap = playerTop < obstacleBottom&& playerBottom > obstacleTop;

			//Used to determine which direction the collision came from
			float bottomCollision = obstacleBottom - playerColliderTransform->y;
			float topCollision = playerBottom - obstacleColliderTransform->y;
			float leftCollision = playerRight - obstacleColliderTransform->x;
			float rightCollision = obstacleRight - playerColliderTransform->x;

			//If they overlap both horizontally and vertically,
			//then they truly overlap.
			if (xOverlap && yOverlap)
			{
				if (topCollision < bottomCollision && topCollision < leftCollision && topCollision < rightCollision)
				{
					pPlayer->SetY(playerColliderTransform->y - topCollision);
				}
				if (bottomCollision < topCollision && bottomCollision < leftCollision && bottomCollision < rightCollision)
				{
					pPlayer->SetY(playerColliderTransform->y + bottomCollision);
				}
				if (leftCollision < rightCollision && leftCollision < topCollision && leftCollision < bottomCollision)
				{
					pPlayer->SetX(playerColliderTransform->x - leftCollision);
				}
				if (rightCollision < leftCollision && rightCollision < topCollision && rightCollision < bottomCollision)
				{
					pPlayer->SetX(playerColliderTransform->x + rightCollision);
				}
			}
		}
	}
}

void GameState2::Render()
{
	SDL_Renderer* pRenderer = Game::GetInstance().GetRenderer();

	SDL_SetRenderDrawColor(pRenderer, 61, 37, 59, 225);
	SDL_RenderClear(pRenderer);

	for (auto const& i : m_objects)
		i.second->Render();
}

void GameState2::Exit()
{
	std::cout << "Exiting GameState..." << std::endl;

	TextureManager::Unload("tiles");
	TextureManager::Unload("otto");
	for (auto& i : m_objects)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_objects.clear();

	SoundManager::StopMusic();
	SoundManager::UnloadMusic("bgm3");
}

void GameState2::Pause()
{
	std::cout << "Pauseing GameState..." << std::endl;
	SoundManager::PauseMusic();
}

void GameState2::Resume()
{
	std::cout << "Resuming GameState..." << std::endl;
	SoundManager::ResumeMusic();
}
// End GameState2

////////////////////////////////////////////
// Begin GameState3 (Level 3)///////////////
void GameState3::Enter() // Used for initialization.
{
	std::cout << "Entering GameState..." << std::endl;

	TextureManager::Load("Assets/Images/Tiles.png", "tiles");
	TextureManager::Load("Assets/Images/Player.png", "player");


	m_objects.emplace("level", new TiledLevel(19, 25, 32, 32, "Assets/Data/Tiledata.txt", "Assets/Data/Level1.txt", "tiles"));
	m_objects.emplace("otto", new Player({ 0, 0, 64, 64 }, { 400, 200, 32, 32 }));

	SoundManager::LoadMusic("Assets/Sound/Music/Blood Lord - The Lord Gives Chase.mp3", "bgm4");
	SoundManager::SetMusicVolume(10);
	SoundManager::PlayMusic("bgm4");
}

void GameState3::Update(float deltaTime)
{
	if (EventManager::KeyPressed(SDL_SCANCODE_X))
	{
		StateManager::ChangeState(new TitleState()); // Change to new TitleState
	}
	else if (EventManager::KeyPressed(SDL_SCANCODE_P))
	{
		StateManager::PushState(new PauseState()); // Add new PauseState
	}
	else
	{
		for (auto const& i : m_objects)
		{
			i.second->Update(deltaTime);
		}

		//Check collision.
		SDL_FRect* playerColliderTransform = m_objects["otto"]->GetDestinationTransform(); // Copies address of player m_dst.

		float playerLeft = playerColliderTransform->x;
		float playerRight = playerColliderTransform->x + playerColliderTransform->w;
		float playerTop = playerColliderTransform->y;
		float playerBottom = playerColliderTransform->y + playerColliderTransform->h;

		Player* pPlayer = static_cast<Player*>(m_objects["otto"]);

		for (unsigned int i = 0; i < static_cast<TiledLevel*>(m_objects["level"])->GetObstacles().size(); i++)
		{
			SDL_FRect* obstacleColliderTransform = static_cast<TiledLevel*>(m_objects["level"])->GetObstacles()[i]->GetDestinationTransform();

			float obstacleLeft = obstacleColliderTransform->x;
			float obstacleRight = obstacleColliderTransform->x + obstacleColliderTransform->w;
			float obstacleTop = obstacleColliderTransform->y;
			float obstacleBottom = obstacleColliderTransform->y + obstacleColliderTransform->h;

			//Check if they overlap horizontally
			//by comparing right vs. left and left vs. right.
			bool xOverlap = playerLeft < obstacleRight&& playerRight > obstacleLeft;

			//Check if they also overlap vertically
			//by comparing top vs. bottom and bottom vs. top.
			bool yOverlap = playerTop < obstacleBottom&& playerBottom > obstacleTop;

			//Used to determine which direction the collision came from
			float bottomCollision = obstacleBottom - playerColliderTransform->y;
			float topCollision = playerBottom - obstacleColliderTransform->y;
			float leftCollision = playerRight - obstacleColliderTransform->x;
			float rightCollision = obstacleRight - playerColliderTransform->x;

			//If they overlap both horizontally and vertically,
			//then they truly overlap.
			if (xOverlap && yOverlap)
			{
				if (topCollision < bottomCollision && topCollision < leftCollision && topCollision < rightCollision)
				{
					pPlayer->SetY(playerColliderTransform->y - topCollision);
				}
				if (bottomCollision < topCollision && bottomCollision < leftCollision && bottomCollision < rightCollision)
				{
					pPlayer->SetY(playerColliderTransform->y + bottomCollision);
				}
				if (leftCollision < rightCollision && leftCollision < topCollision && leftCollision < bottomCollision)
				{
					pPlayer->SetX(playerColliderTransform->x - leftCollision);
				}
				if (rightCollision < leftCollision && rightCollision < topCollision && rightCollision < bottomCollision)
				{
					pPlayer->SetX(playerColliderTransform->x + rightCollision);
				}
			}
		}
	}
}

void GameState3::Render()
{
	SDL_Renderer* pRenderer = Game::GetInstance().GetRenderer();

	SDL_SetRenderDrawColor(pRenderer, 61, 37, 59, 225);
	SDL_RenderClear(pRenderer);

	for (auto const& i : m_objects)
		i.second->Render();
}

void GameState3::Exit()
{
	std::cout << "Exiting GameState..." << std::endl;

	TextureManager::Unload("tiles");
	TextureManager::Unload("player");
	for (auto& i : m_objects)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_objects.clear();

	SoundManager::StopMusic();
	SoundManager::UnloadMusic("bgm4");
}

void GameState3::Pause()
{
	std::cout << "Pauseing GameState..." << std::endl;
	SoundManager::PauseMusic();
}

void GameState3::Resume()
{
	std::cout << "Resuming GameState..." << std::endl;
	SoundManager::ResumeMusic();
}
// End GameState3

////////////////////////////////////////////
// Begin EndState ////////////////////////
void EndState::Enter()
{
	std::cout << "Entering EndState..." << std::endl;
	SoundManager::LoadMusic("Assets/Sound/Music/Blood Lord - A New Day.mp3", "bgm5");
	SoundManager::SetMusicVolume(20);
	SoundManager::PlayMusic("bgm5");

	TextureManager::Load("Assets/Images/Buttons/play.png", "play");
	TextureManager::Load("Assets/Images/Buttons/exit.png", "exit");
	TextureManager::Load("Assets/Images/gameover.png", "bg");

	int buttonWidth = 400;
	int buttonHeight = 100;
	float buttonX = Game::GetInstance().kWidth / 2 - buttonWidth / 2.0f;
	float buttonY = Game::GetInstance().kHeight / 2 - buttonHeight / 2.0f;

	m_objects.emplace("bg", new Background("bg"));
	m_objects.emplace("play", new PlayButton({ 0, 0, buttonWidth, buttonHeight }, { buttonX, buttonY, (float)buttonWidth, (float)buttonHeight }, "play"));
	m_objects.emplace("exit", new ExitButton({ 0, 0, buttonWidth, buttonHeight }, { 200, 400, (float)buttonWidth, (float)buttonHeight }, "exit"));

}

void EndState::Update(float deltaTime)
{
	for (auto const& i : m_objects)
	{
		i.second->Update(deltaTime);
		if (StateManager::StateChanging())
			return;
	}
}

void EndState::Render()
{
	//SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 255, 255);
	SDL_RenderClear(Game::GetInstance().GetRenderer());

	for (auto const& i : m_objects)
		i.second->Render();
}

void EndState::Exit()
{
	std::cout << "Exiting TitleState..." << std::endl;
	SoundManager::StopMusic();
	SoundManager::UnloadMusic("bgm5");

	TextureManager::Unload("gameover");
	TextureManager::Unload("play");
	TextureManager::Unload("exit");
	for (auto& i : m_objects)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_objects.clear();
}
// End EndState
