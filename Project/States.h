#pragma once
#include <SDL.h>
#include <map>
#include <string>
#include <vector>
#include "Collectables.h"
class Enemy;
class Player;
class GameObject;

class State // This is the abstract base class for all states
{
public:
	State() = default;
	virtual ~State() = default;

	virtual void Enter() = 0; // = 0 means pure virtual - must be defined in subclass
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;
	virtual void Pause() { }
	virtual void Resume() { }
	SDL_FRect* getPlayerPosition() { return m_objects["otto"]->GetDestinationTransform(); }
	Player* Otto;
	Collectables* m_key;
	Collectables* m_key2;
	Collectables* m_potion;
	
	std::map<std::string, GameObject*> m_objects;
	std::vector<Collectables*> m_collectables;
	std::vector<Enemy*> m_enemy;
	State* statepointer;

	int counter = 0;
};

class TitleState : public State
{
public:
	virtual void Enter() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Exit() override;
};

class GameState : public State
{
	static const int kRectangeSpeed = 300;
	SDL_FRect m_RectangleTransform;
	int lives = 3;
	//Collectables* m_key;

public:
	
	virtual void Enter() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Exit() override;
	virtual void Resume() override;
	virtual void Pause() override;
};

class PauseState : public State
{
public:
	virtual void Enter() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Exit() override;
};

class GameState2 : public State
{

public:
	virtual void Enter() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Exit() override;
	virtual void Resume() override;
	virtual void Pause() override;
};

class GameState3 : public State
{

public:
	virtual void Enter() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Exit() override;
	virtual void Resume() override;
	virtual void Pause() override;
};

class EndState : public State
{
public:
	virtual void Enter() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Exit() override;
};