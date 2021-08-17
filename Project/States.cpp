#include <iostream>

#include "CollisionManager.h"
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
#include "Boss.h"
#include"Potion.h"

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

	TextureManager::Load("Assets/Images/grave.png", "grave");
	TextureManager::Load("Assets/Images/3.png", "3h");
	TextureManager::Load("Assets/Images/2.png", "2h");
	TextureManager::Load("Assets/Images/1.png", "1h");

	m_objects.emplace("level1", new TiledLevel(19, 25, 32, 32, "Assets/Data/Level1 Data.txt", "Assets/Data/Level1.txt", "grave"));
	Otto = new Player({ 0, 0, 64, 64 }, { 60, 32, 32, 32 });
	m_objects.emplace("otto", Otto);
	//Enemies 6 // 
	m_enemy.push_back(new Enemy({ 0, 0, 80, 80 }, { 350, 32, 64, 64 }, 100, 15, this));
	m_enemy.push_back(new Enemy({ 0, 0, 80, 80 }, { 550, 350, 64, 64 }, 100, 15, this));
	m_enemy.push_back(new Enemy({ 0, 0, 80, 80 }, { 190, 330, 64, 64 }, 100, 15, this));

	m_collectables.push_back(new Collectables({ 0, 0, 32,32 }, { 80, 450, 32,32 }, CollectableType::key));
	m_collectables.push_back(new Collectables({ 0, 0, 32,32 }, { 350, 50, 32,32 }, CollectableType::key));
	m_collectables.push_back(new Collectables({ 0, 0, 32,32 }, { 600, 500, 32,32 }, CollectableType::key));
	m_collectables.push_back(new Collectables({ 0, 0, 32,32 }, { 700, 400, 32,32 }, CollectableType::p1));
	m_collectables.push_back(new Collectables({ 0, 0, 32,32 }, { 60, 520, 32,32 }, CollectableType::p1));
	m_collectables.shrink_to_fit();

	SoundManager::LoadMusic("Assets/Sound/Music/Blood Lord - A Long Journey.mp3", "bgm2");
	SoundManager::SetMusicVolume(10);
	SoundManager::PlayMusic("bgm2");
}

void GameState::Update(float deltaTime)
{
	counter++;
	std::cout << m_objects["otto"]->GetDestinationTransform()->x << "," << m_objects["otto"]->GetDestinationTransform()->y << std::endl;
	if (EventManager::KeyPressed(SDL_SCANCODE_F1))
	{
		StateManager::ChangeState(new TitleState());
	}
	else if (EventManager::KeyPressed(SDL_SCANCODE_F2))
	{
		StateManager::ChangeState(new GameState());
	}
	else if (EventManager::KeyPressed(SDL_SCANCODE_F3))
	{
		StateManager::ChangeState(new GameState2());
	}
	else if (EventManager::KeyPressed(SDL_SCANCODE_F4))
	{
		StateManager::ChangeState(new GameState3());
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
		for (auto i : m_enemy)
		{
			i->Update(deltaTime);
		}

		//Check collision.
		Player* pPlayer = static_cast<Player*>(m_objects["otto"]);
		SDL_FRect* playerColliderTransform = m_objects["otto"]->GetDestinationTransform(); // Copies address of player m_dst.
		float playerLeft = playerColliderTransform->x;
		float playerRight = playerColliderTransform->x + playerColliderTransform->w;
		float playerTop = playerColliderTransform->y;
		float playerBottom = playerColliderTransform->y + playerColliderTransform->h;

		for (unsigned int i = 0; i < static_cast<TiledLevel*>(m_objects["level1"])->GetObstacles().size(); i++)
		{
			SDL_FRect* obstacleColliderTransform = static_cast<TiledLevel*>(m_objects["level1"])->GetObstacles()[i]->GetDestinationTransform();

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
			//m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(), (Collectables key) = > {if m_key->GetDestinationTransform() == m_objects["otto"]->GetDestinationTransform()})
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

		if (m_enemy.size() < 0)
		{
			for (int i = 0; i < m_enemy.size(); i++)
			{
				enemyMove(m_enemy[i]);
				enemyAvoid(m_enemy[i]);
				//move individually
				//check collision individually
				/*Enemy* pEnemy = m_enemy[i];
				SDL_FRect* enemyColliderTransform = pEnemy->GetDestinationTransform();
				int enemyLeft = enemyColliderTransform->x;
				int enemyRight = enemyColliderTransform->x + enemyColliderTransform->w;
				int enemyTop = enemyColliderTransform->y;
				int enemyBottom = enemyColliderTransform->y + enemyColliderTransform->h;

				for (unsigned int i = 0; i < static_cast<TiledLevel*>(m_objects["level1"])->GetObstacles().size(); i++)
				{
					SDL_FRect* obstacleColliderTransform = static_cast<TiledLevel*>(m_objects["level1"])->GetObstacles()[i]->GetDestinationTransform();

					float obstacleLeft = obstacleColliderTransform->x;
					float obstacleRight = obstacleColliderTransform->x + obstacleColliderTransform->w;
					float obstacleTop = obstacleColliderTransform->y;
					float obstacleBottom = obstacleColliderTransform->y + obstacleColliderTransform->h;

					bool xOverlap = enemyLeft < obstacleRight&& enemyRight > obstacleLeft;

					bool yOverlap = enemyTop < obstacleBottom&& enemyBottom > obstacleTop;

					float bottomCollision = obstacleBottom - enemyColliderTransform->y;
					float topCollision = enemyBottom - obstacleColliderTransform->y;
					float leftCollision = enemyRight - obstacleColliderTransform->x;
					float rightCollision = obstacleRight - enemyColliderTransform->x;

					if (xOverlap && yOverlap)
					{
						if (topCollision < bottomCollision && topCollision < leftCollision && topCollision < rightCollision)
						{
							pEnemy->SetY(enemyColliderTransform->y - topCollision);
						}
						if (bottomCollision < topCollision && bottomCollision < leftCollision && bottomCollision < rightCollision)
						{
							pEnemy->SetY(enemyColliderTransform->y + bottomCollision);
						}
						if (leftCollision < rightCollision && leftCollision < topCollision && leftCollision < bottomCollision)
						{
							pEnemy->SetX(enemyColliderTransform->x - leftCollision);
						}
						if (rightCollision < leftCollision && rightCollision < topCollision && rightCollision < bottomCollision)
						{
							pEnemy->SetX(enemyColliderTransform->x + rightCollision);
						}
					}
				}*/
			}
		}

		if (!m_collectables.empty())
		{

			for (int i = 0; i < m_collectables.size(); i++)
			{
				SDL_Rect  collectable1 = { m_collectables[i]->GetDestinationTransform()->x, m_collectables[i]->GetDestinationTransform()->y, m_collectables[i]->GetDestinationTransform()->w, m_collectables[i]->GetDestinationTransform()->h };

				SDL_Rect player = { m_objects["otto"]->GetDestinationTransform()->x, m_objects["otto"]->GetDestinationTransform()->y, m_objects["otto"]->GetDestinationTransform()->w, m_objects["otto"]->GetDestinationTransform()->h };

				if (SDL_HasIntersection(&collectable1, &player))
				{
						if (m_collectables[i]->getCollectableType() == CollectableType::p1)
					{
						ottoGainLife();
					}
					delete m_collectables[i];
					m_collectables[i] = nullptr;
					m_collectables.erase(m_collectables.begin() + i);
					m_collectables.shrink_to_fit();
				}

			}
		}

		const SDL_Rect player = { Otto->GetDestinationTransform()->x, Otto->GetDestinationTransform()->y,
		Otto->GetDestinationTransform()->w, Otto->GetDestinationTransform()->h };

		for (int i = 0; i < m_enemy.size(); i++)
		{
			const SDL_Rect enemy = { m_enemy[i]->GetDestinationTransform()->x, m_enemy[i]->GetDestinationTransform()->y,
			m_enemy[i]->GetDestinationTransform()->w, m_enemy[i]->GetDestinationTransform()->h };
			if (SDL_HasIntersection(&player, &enemy) && EventManager::KeyHeld(SDL_SCANCODE_SPACE))
			{
				delete m_enemy[i];
				m_enemy[i] = nullptr;
				m_enemy.erase(m_enemy.begin() + i);
				m_enemy.shrink_to_fit();
			}
			if (SDL_HasIntersection(&player, &enemy) && counter % 400 == 0)
			{
				ottoLoseLife();
			}

			if (counter > 400)
			{
				counter -= 400;
			}
		}

		if (m_objects["otto"]->GetDestinationTransform()->x == 704 &&
			m_objects["otto"]->GetDestinationTransform()->y == 320 && m_collectables.empty())
		{
			GameState::Exit();
			StateManager::ChangeState(new GameState2);
		}
	}
}

void GameState::Render()
{
	SDL_Renderer* pRenderer = Game::GetInstance().GetRenderer();

	SDL_SetRenderDrawColor(pRenderer, 58, 89, 65, 225);
	SDL_RenderClear(pRenderer);

	for (auto const& i : m_objects)
		i.second->Render();
	if (!m_collectables.empty())
	{
		for (int i = 0; i < m_collectables.size(); i++)
		{
			m_collectables[i]->Render();
		}
	}
	if (!m_enemy.empty())
	{
		for (int i = 0; i < m_enemy.size(); i++)
		{
			m_enemy[i]->Render();
		}
	}




	//m_potion->Render();
	///////////LIVES///////////////
	SDL_FRect* playerPos = m_objects["otto"]->GetDestinationTransform();
	int ottoX = playerPos->x;
	int ottoY = playerPos->y;
	const SDL_Rect m_h1Src = { 0, 0, 25, 25 };
	const SDL_Rect m_h2Src = { 0, 0, 50, 25 };
	const SDL_Rect m_h3Src = { 0, 0, 75, 25 };
	const SDL_FRect m_h1Dst = { ottoX , ottoY - 20, 25, 25 };
	const SDL_FRect m_h2Dst = { ottoX - 10, ottoY - 20, 50, 25 };
	const SDL_FRect m_h3Dst = { ottoX - 22, ottoY - 20, 75, 25 };

	if (ottoLives == 3)
	{
		SDL_RenderCopyExF(Game::GetInstance().GetRenderer(),
			TextureManager::GetTexture("3h"), &m_h3Src, &m_h3Dst, 0, 0, SDL_FLIP_NONE);
	}
	else if (ottoLives == 2)
	{
		SDL_RenderCopyExF(Game::GetInstance().GetRenderer(),
			TextureManager::GetTexture("2h"), &m_h2Src, &m_h2Dst, 0, 0, SDL_FLIP_NONE);
	}
	else if (ottoLives == 1)
	{
		SDL_RenderCopyExF(Game::GetInstance().GetRenderer(),
			TextureManager::GetTexture("1h"), &m_h1Src, &m_h1Dst, 0, 0, SDL_FLIP_NONE);
	}
	else if (ottoLives == 0)
	{
		StateManager::ChangeState(new EndState());
	}
}

void GameState::Exit()
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
	m_enemy.clear();
	m_collectables.clear();

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

	m_objects.emplace("level2", new TiledLevel(19, 25, 32, 32, "Assets/Data/Level2 Data.txt", "Assets/Data/Level2.txt", "tiles"));
	Otto = new Player({ 0, 0, 64, 64 }, { 32, 512, 32, 32 });
	m_objects.emplace("otto", Otto);

	m_enemy.push_back(new Enemy({ 0, 0, 80, 80 }, { 735, 544, 64, 64 }, 100, 15, this));
	m_enemy.push_back(new Enemy({ 0, 0, 80, 80 }, { 21, 288, 64, 64 }, 100, 15, this));
	m_enemy.push_back(new Enemy({ 0, 0, 80, 80 }, { 429, 96, 64, 64 }, 100, 15, this));

	m_collectables.push_back(new Collectables({ 0, 0, 32, 32 }, { 672, 320, 32, 32 }, CollectableType::key));
	m_collectables.push_back(new Collectables({ 0, 0, 32, 32 }, { 512, 480, 32, 32 }, CollectableType::key));
	m_collectables.push_back(new Collectables({ 0, 0, 32, 32 }, { 32, 160, 32, 32 }, CollectableType::key));
	m_collectables.push_back(new Collectables({ 0, 0, 32, 32 }, { 512, 288, 32, 32 }, CollectableType::p1));
	m_collectables.push_back(new Collectables({ 0, 0, 32, 32 }, { 736, 96, 32, 32 }, CollectableType::p1));
	m_collectables.shrink_to_fit();

	SoundManager::LoadMusic("Assets/Sound/Music/Blood Lord - At the Gates.mp3", "bgm3");
	SoundManager::SetMusicVolume(10);
	SoundManager::PlayMusic("bgm3");
}

void GameState2::Update(float deltaTime)
{
	counter++;
	std::cout << m_objects["otto"]->GetDestinationTransform()->x << "," << m_objects["otto"]->GetDestinationTransform()->y << std::endl;
	if (EventManager::KeyPressed(SDL_SCANCODE_F1))
	{
		StateManager::ChangeState(new TitleState());
	}
	else if (EventManager::KeyPressed(SDL_SCANCODE_F2))
	{
		StateManager::ChangeState(new GameState());
	}
	else if (EventManager::KeyPressed(SDL_SCANCODE_F3))
	{
		StateManager::ChangeState(new GameState2());
	}
	else if (EventManager::KeyPressed(SDL_SCANCODE_F4))
	{
		StateManager::ChangeState(new GameState3());
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
		for (auto i : m_enemy)
		{
			i->Update(deltaTime);
		}


		Player* pPlayer = static_cast<Player*>(m_objects["otto"]);
		SDL_FRect* playerColliderTransform = m_objects["otto"]->GetDestinationTransform(); // Copies address of player m_dst.
		float playerLeft = playerColliderTransform->x;
		float playerRight = playerColliderTransform->x + playerColliderTransform->w;
		float playerTop = playerColliderTransform->y;
		float playerBottom = playerColliderTransform->y + playerColliderTransform->h;

		for (unsigned int i = 0; i < static_cast<TiledLevel*>(m_objects["level2"])->GetObstacles().size(); i++)
		{
			SDL_FRect* obstacleColliderTransform = static_cast<TiledLevel*>(m_objects["level2"])->GetObstacles()[i]->GetDestinationTransform();

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
			//m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(), (Collectables key) = > {if m_key->GetDestinationTransform() == m_objects["otto"]->GetDestinationTransform()})
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

		if (m_enemy.size() < 0)
		{
			Enemy* pEnemy = m_enemy.front();
			SDL_FRect* enemyColliderTransform = pEnemy->GetDestinationTransform();
			int enemyLeft = enemyColliderTransform->x;
			int enemyRight = enemyColliderTransform->x + enemyColliderTransform->w;
			int enemyTop = enemyColliderTransform->y;
			int enemyBottom = enemyColliderTransform->y + enemyColliderTransform->h;

			for (unsigned int i = 0; i < static_cast<TiledLevel*>(m_objects["level1"])->GetObstacles().size(); i++)
			{
				SDL_FRect* obstacleColliderTransform = static_cast<TiledLevel*>(m_objects["level1"])->GetObstacles()[i]->GetDestinationTransform();

				float obstacleLeft = obstacleColliderTransform->x;
				float obstacleRight = obstacleColliderTransform->x + obstacleColliderTransform->w;
				float obstacleTop = obstacleColliderTransform->y;
				float obstacleBottom = obstacleColliderTransform->y + obstacleColliderTransform->h;

				bool xOverlap = enemyLeft < obstacleRight&& enemyRight > obstacleLeft;

				bool yOverlap = enemyTop < obstacleBottom&& enemyBottom > obstacleTop;

				float bottomCollision = obstacleBottom - enemyColliderTransform->y;
				float topCollision = enemyBottom - obstacleColliderTransform->y;
				float leftCollision = enemyRight - obstacleColliderTransform->x;
				float rightCollision = obstacleRight - enemyColliderTransform->x;

				if (xOverlap && yOverlap)
				{
					if (topCollision < bottomCollision && topCollision < leftCollision && topCollision < rightCollision)
					{
						pEnemy->SetY(enemyColliderTransform->y - topCollision);
					}
					if (bottomCollision < topCollision && bottomCollision < leftCollision && bottomCollision < rightCollision)
					{
						pEnemy->SetY(enemyColliderTransform->y + bottomCollision);
					}
					if (leftCollision < rightCollision && leftCollision < topCollision && leftCollision < bottomCollision)
					{
						pEnemy->SetX(enemyColliderTransform->x - leftCollision);
					}
					if (rightCollision < leftCollision && rightCollision < topCollision && rightCollision < bottomCollision)
					{
						pEnemy->SetX(enemyColliderTransform->x + rightCollision);
					}
				}
			}
		}

		if (!m_collectables.empty())
		{

			for (int i = 0; i < m_collectables.size(); i++)
			{
				SDL_Rect  collectable1 = { m_collectables[i]->GetDestinationTransform()->x, m_collectables[i]->GetDestinationTransform()->y, m_collectables[i]->GetDestinationTransform()->w, m_collectables[i]->GetDestinationTransform()->h };

				SDL_Rect player = { m_objects["otto"]->GetDestinationTransform()->x, m_objects["otto"]->GetDestinationTransform()->y, m_objects["otto"]->GetDestinationTransform()->w, m_objects["otto"]->GetDestinationTransform()->h };

				if (SDL_HasIntersection(&collectable1, &player))
				{
					if (m_collectables[i]->getCollectableType() == CollectableType::p1)
					{
						ottoGainLife();
					}
					delete m_collectables[i];
					m_collectables[i] = nullptr;
					m_collectables.erase(m_collectables.begin() + i);
					m_collectables.shrink_to_fit();
					
				}

			}
		}

		const SDL_Rect player = { Otto->GetDestinationTransform()->x, Otto->GetDestinationTransform()->y,
		Otto->GetDestinationTransform()->w, Otto->GetDestinationTransform()->h };

		for (int i = 0; i < m_enemy.size(); i++)
		{
			const SDL_Rect enemy = { m_enemy[i]->GetDestinationTransform()->x, m_enemy[i]->GetDestinationTransform()->y,
			m_enemy[i]->GetDestinationTransform()->w, m_enemy[i]->GetDestinationTransform()->h };
			if (SDL_HasIntersection(&player, &enemy) && EventManager::KeyHeld(SDL_SCANCODE_SPACE))
			{
				delete m_enemy[i];
				m_enemy[i] = nullptr;
				m_enemy.erase(m_enemy.begin() + i);
				m_enemy.shrink_to_fit();
			}
			if (SDL_HasIntersection(&player, &enemy) && counter % 400 == 0)
			{
				ottoLoseLife();
			}

			if (counter > 400)
			{
				counter -= 400;
			}
		}
		if (m_objects["otto"]->GetDestinationTransform()->x == 736 &&
			m_objects["otto"]->GetDestinationTransform()->y == 32 && m_collectables.empty())
		{
			GameState2::Exit();
			StateManager::ChangeState(new GameState3);
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

	if (!m_collectables.empty())
	{
		for (int i = 0; i < m_collectables.size(); i++)
		{
			m_collectables[i]->Render();
		}
	}
	if (!m_enemy.empty())
	{
		for (int i = 0; i < m_enemy.size(); i++)
		{
			m_enemy[i]->Render();
		}
	}

	SDL_FRect* playerPos = m_objects["otto"]->GetDestinationTransform();
	int ottoX = playerPos->x;
	int ottoY = playerPos->y;
	const SDL_Rect m_h1Src = { 0, 0, 25, 25 };
	const SDL_Rect m_h2Src = { 0, 0, 50, 25 };
	const SDL_Rect m_h3Src = { 0, 0, 75, 25 };
	const SDL_FRect m_h1Dst = { ottoX , ottoY - 20, 25, 25 };
	const SDL_FRect m_h2Dst = { ottoX - 10, ottoY - 20, 50, 25 };
	const SDL_FRect m_h3Dst = { ottoX - 22, ottoY - 20, 75, 25 };
	if (ottoLives == 3)
	{
		SDL_RenderCopyExF(Game::GetInstance().GetRenderer(),
			TextureManager::GetTexture("3h"), &m_h3Src, &m_h3Dst, 0, 0, SDL_FLIP_NONE);
	}
	else if (ottoLives == 2)
	{
		SDL_RenderCopyExF(Game::GetInstance().GetRenderer(),
			TextureManager::GetTexture("2h"), &m_h2Src, &m_h2Dst, 0, 0, SDL_FLIP_NONE);
	}
	else if (ottoLives == 1)
	{
		SDL_RenderCopyExF(Game::GetInstance().GetRenderer(),
			TextureManager::GetTexture("1h"), &m_h1Src, &m_h1Dst, 0, 0, SDL_FLIP_NONE);
	}
	else if (ottoLives == 0)
	{
		StateManager::ChangeState(new EndState());
	}
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

	m_objects.emplace("level3", new TiledLevel(19, 25, 32, 32, "Assets/Data/Level3 Data.txt", "Assets/Data/Level3.txt", "tiles"));
	Otto = new Player({ 0, 0, 64, 64 }, { 32, 544, 32, 32 });
	m_objects.emplace("otto", Otto);

	boss = new Boss({ 0,0,64,64 }, { 683,366,64,64 });
	m_objects.emplace("boss", boss);

	SoundManager::LoadMusic("Assets/Sound/Music/Blood Lord - The Lord Gives Chase.mp3", "bgm4");
	SoundManager::SetMusicVolume(10);
	SoundManager::PlayMusic("bgm4");
}

void GameState3::Update(float deltaTime)
{
	std::cout << m_objects["boss"]->GetDestinationTransform()->x << "," << m_objects["boss"]->GetDestinationTransform()->y << std::endl;
	if (EventManager::KeyPressed(SDL_SCANCODE_P))
	{
		StateManager::PushState(new PauseState()); // Add new PauseState
	}
	else
	{
		boss->Update(deltaTime);
		for (auto const& i : m_objects)
		{
			i.second->Update(deltaTime);
		}

		//Check collision.
		SDL_FRect* playerColliderTransform = m_objects["otto"]->GetDestinationTransform(); // Copies address of player m_dst.
		SDL_FRect* bossColliderTransform = boss->GetDestinationTransform();

		Player* pPlayer = static_cast<Player*>(m_objects["otto"]);

		for (unsigned int i = 0; i < static_cast<TiledLevel*>(m_objects["level3"])->GetObstacles().size(); i++)
		{
			float playerLeft = playerColliderTransform->x;
			float playerRight = playerColliderTransform->x + playerColliderTransform->w;
			float playerTop = playerColliderTransform->y;
			float playerBottom = playerColliderTransform->y + playerColliderTransform->h;
			SDL_FRect* obstacleColliderTransform = static_cast<TiledLevel*>(m_objects["level3"])->GetObstacles()[i]->GetDestinationTransform();

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
			//m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(), (Collectables key) = > {if m_key->GetDestinationTransform() == m_objects["otto"]->GetDestinationTransform()})
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
		for (unsigned int i = 0; i < static_cast<TiledLevel*>(m_objects["level3"])->GetObstacles().size(); i++)
		{
			float bossLeft = bossColliderTransform->x;
			float bossRight = bossColliderTransform->x + bossColliderTransform->w;
			float bossTop = bossColliderTransform->y;
			float bossBottom = bossColliderTransform->y + bossColliderTransform->h;

			SDL_FRect* obstacleColliderTransform = static_cast<TiledLevel*>(m_objects["level3"])->GetObstacles()[i]->GetDestinationTransform();

			float obstacleLeft = obstacleColliderTransform->x;
			float obstacleRight = obstacleColliderTransform->x + obstacleColliderTransform->w;
			float obstacleTop = obstacleColliderTransform->y;
			float obstacleBottom = obstacleColliderTransform->y + obstacleColliderTransform->h;

			//Check if they overlap horizontally
			//by comparing right vs. left and left vs. right.
			bool xOverlap = bossLeft < obstacleRight&& bossRight > obstacleLeft;

			//Check if they also overlap vertically
			//by comparing top vs. bottom and bottom vs. top.
			bool yOverlap = bossTop < obstacleBottom&& bossBottom > obstacleTop;

			//Used to determine which direction the collision came from
			float bottomCollision = obstacleBottom - bossColliderTransform->y;
			float topCollision = bossBottom - obstacleColliderTransform->y;
			float leftCollision = bossRight - obstacleColliderTransform->x;
			float rightCollision = obstacleRight - bossColliderTransform->x;

			//If they overlap both horizontally and vertically,
			//then they truly overlap.
			if (xOverlap && yOverlap)
			{
				if (topCollision < bottomCollision && topCollision < leftCollision && topCollision < rightCollision)
				{
					boss->SetY(bossColliderTransform->y - topCollision);
				}
				if (bottomCollision < topCollision && bottomCollision < leftCollision && bottomCollision < rightCollision)
				{
					boss->SetY(bossColliderTransform->y + bottomCollision);
				}
				if (leftCollision < rightCollision && leftCollision < topCollision && leftCollision < bottomCollision)
				{
					boss->SetX(bossColliderTransform->x - leftCollision);
				}
				if (rightCollision < leftCollision && rightCollision < topCollision && rightCollision < bottomCollision)
				{
					boss->SetX(bossColliderTransform->x + rightCollision);
				}
			}
		}
	

		const SDL_Rect bossRect = {boss->GetDestinationTransform()->x, boss->GetDestinationTransform()->y, 
			boss->GetDestinationTransform()->w, boss->GetDestinationTransform()->h};
		const SDL_Rect ottoRect = {Otto->GetDestinationTransform()->x, Otto->GetDestinationTransform()->y,
			Otto->GetDestinationTransform()->w, Otto->GetDestinationTransform()->h};

		if (CollisionManager::Distance(boss->GetDestinationTransform()->x, Otto->GetDestinationTransform()->x,
			boss->GetDestinationTransform()->y, Otto->GetDestinationTransform()->y) < 200)
		{
			boss->setBossState(Boss::BossState::kFollowing);
			
			bossFollow();
			if (SDL_HasIntersection(&bossRect, &ottoRect))
			{
				boss->setBossState(Boss::BossState::kAttacking);
			}	
		}
		else
		{
			boss->setBossState(Boss::BossState::kRunning);
			boss->Wander();
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

	boss->Render();

	SDL_FRect* playerPos = m_objects["otto"]->GetDestinationTransform();
	int ottoX = playerPos->x;
	int ottoY = playerPos->y;
	const SDL_Rect m_h1Src = { 0, 0, 25, 25 };
	const SDL_Rect m_h2Src = { 0, 0, 50, 25 };
	const SDL_Rect m_h3Src = { 0, 0, 75, 25 };
	const SDL_FRect m_h1Dst = { ottoX , ottoY - 20, 25, 25 };
	const SDL_FRect m_h2Dst = { ottoX - 10, ottoY - 20, 50, 25 };
	const SDL_FRect m_h3Dst = { ottoX - 22, ottoY - 20, 75, 25 };
	if (ottoLives == 3)
	{
		SDL_RenderCopyExF(Game::GetInstance().GetRenderer(),
			TextureManager::GetTexture("3h"), &m_h3Src, &m_h3Dst, 0, 0, SDL_FLIP_NONE);
	}
	else if (ottoLives == 2)
	{
		SDL_RenderCopyExF(Game::GetInstance().GetRenderer(),
			TextureManager::GetTexture("2h"), &m_h2Src, &m_h2Dst, 0, 0, SDL_FLIP_NONE);
	}
	else if (ottoLives == 1)
	{
		SDL_RenderCopyExF(Game::GetInstance().GetRenderer(),
			TextureManager::GetTexture("1h"), &m_h1Src, &m_h1Dst, 0, 0, SDL_FLIP_NONE);
	}
	else if (ottoLives == 0)
	{
		StateManager::ChangeState(new EndState());
	}
}

void GameState3::Exit()
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
	SoundManager::LoadMusic("Assets/Sound/Music/Blood Lord - An Ancient Relic.mp3", "bgm5");
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
// End EndState

////////////////////////////////////////////
// Begin WinState ////////////////////////
void WinState::Enter()
{
	std::cout << "Entering WinState..." << std::endl;
	SoundManager::LoadMusic("Assets/Sound/Music/Blood Lord - A New Day.mp3", "bgm6");
	SoundManager::SetMusicVolume(20);
	SoundManager::PlayMusic("bgm6");

	TextureManager::Load("Assets/Images/Buttons/play.png", "play");
	TextureManager::Load("Assets/Images/Buttons/exit.png", "exit");
	TextureManager::Load("Assets/Images/win.png", "bg");

	int buttonWidth = 400;
	int buttonHeight = 100;
	float buttonX = Game::GetInstance().kWidth / 2 - buttonWidth / 2.0f;
	float buttonY = Game::GetInstance().kHeight / 2 - buttonHeight / 2.0f;

	m_objects.emplace("bg", new Background("bg"));
	m_objects.emplace("play", new PlayButton({ 0, 0, buttonWidth, buttonHeight }, { buttonX, buttonY, (float)buttonWidth, (float)buttonHeight }, "play"));
	m_objects.emplace("exit", new ExitButton({ 0, 0, buttonWidth, buttonHeight }, { 200, 400, (float)buttonWidth, (float)buttonHeight }, "exit"));

}

void WinState::Update(float deltaTime)
{
	for (auto const& i : m_objects)
	{
		i.second->Update(deltaTime);
		if (StateManager::StateChanging())
			return;
	}
}

void WinState::Render()
{
	//SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 255, 255);
	SDL_RenderClear(Game::GetInstance().GetRenderer());

	for (auto const& i : m_objects)
		i.second->Render();
}

void WinState::Exit()
{
	std::cout << "Exiting TitleState..." << std::endl;
	SoundManager::StopMusic();
	SoundManager::UnloadMusic("bgm6");

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
// End WinState

//extra space saving functions

void State::bossFollow()
{
	if (boss->GetDestinationTransform()->x < Otto->GetDestinationTransform()->x) //enemy is to the left of player
	{
		boss->GetDestinationTransform()->x += boss->getBossSpeed() * 2;
		boss->setBossFacingLeft(true);
	}
	if (boss->GetDestinationTransform()->x > Otto->GetDestinationTransform()->x)//enemy is to the right of player
	{
		boss->GetDestinationTransform()->x -= boss->getBossSpeed() * 2;
		boss->setBossFacingLeft(false);
	}
	if (boss->GetDestinationTransform()->y < Otto->GetDestinationTransform()->y) //enemy is above player
	{
		boss->GetDestinationTransform()->y += boss->getBossSpeed() * 2;
	}
	if (boss->GetDestinationTransform()->y > Otto->GetDestinationTransform()->y) //enemy is below player
	{
		boss->GetDestinationTransform()->y -= boss->getBossSpeed() * 2;
	}
}

void State::enemyMove(Enemy* enemy)
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
		enemy->GetDestinationTransform()->x -= enemy->getSpeed();
	}
	else if (roll == 2)
	{
		enemy->GetDestinationTransform()->x += enemy->getSpeed();
	}
	else if (roll == 3)
	{
		enemy->GetDestinationTransform()->y -= enemy->getSpeed();
	}
	if (roll == 4)
	{
		enemy->GetDestinationTransform()->y += enemy->getSpeed();
	}
}

void State::enemyAvoid(Enemy* enemy)
{
	SDL_FRect* enemyColliderTransform = enemy->GetDestinationTransform();
	int enemyLeft = enemyColliderTransform->x;
	int enemyRight = enemyColliderTransform->x + enemyColliderTransform->w;
	int enemyTop = enemyColliderTransform->y;
	int enemyBottom = enemyColliderTransform->y + enemyColliderTransform->h;

	for (unsigned int i = 0; i < static_cast<TiledLevel*>(m_objects["level1"])->GetObstacles().size(); i++)
	{
		SDL_FRect* obstacleColliderTransform = static_cast<TiledLevel*>(m_objects["level1"])->GetObstacles()[i]->GetDestinationTransform();

		float obstacleLeft = obstacleColliderTransform->x;
		float obstacleRight = obstacleColliderTransform->x + obstacleColliderTransform->w;
		float obstacleTop = obstacleColliderTransform->y;
		float obstacleBottom = obstacleColliderTransform->y + obstacleColliderTransform->h;

		bool xOverlap = enemyLeft < obstacleRight&& enemyRight > obstacleLeft;

		bool yOverlap = enemyTop < obstacleBottom&& enemyBottom > obstacleTop;

		float bottomCollision = obstacleBottom - enemyColliderTransform->y;
		float topCollision = enemyBottom - obstacleColliderTransform->y;
		float leftCollision = enemyRight - obstacleColliderTransform->x;
		float rightCollision = obstacleRight - enemyColliderTransform->x;

		if (xOverlap && yOverlap)
		{
			if (topCollision < bottomCollision && topCollision < leftCollision && topCollision < rightCollision)
			{
				enemy->SetY(enemyColliderTransform->y - topCollision);
			}
			if (bottomCollision < topCollision && bottomCollision < leftCollision && bottomCollision < rightCollision)
			{
				enemy->SetY(enemyColliderTransform->y + bottomCollision);
			}
			if (leftCollision < rightCollision && leftCollision < topCollision && leftCollision < bottomCollision)
			{
				enemy->SetX(enemyColliderTransform->x - leftCollision);
			}
			if (rightCollision < leftCollision && rightCollision < topCollision && rightCollision < bottomCollision)
			{
				enemy->SetX(enemyColliderTransform->x + rightCollision);
			}
		}
	}
}
