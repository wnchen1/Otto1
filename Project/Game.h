#pragma once
#include <SDL.h>

class Game
{
public:
	static const int kWidth = 800;
	static const int kHeight = 608;

public:
	static Game& GetInstance();
	int Init(const char* title, int xPos, int yPos);
	bool IsRunning();
	void Quit();
	void HandleEvents();
	void Update(float deltaTime);
	void Render();
	void Clean();

	SDL_Window* GetWindow() { return m_pWindow; }
	SDL_Renderer* GetRenderer() { return m_pRenderer; }

private:
	Game(); // private constructor

	bool m_running;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;

	const Uint8* m_keyStates;
};

