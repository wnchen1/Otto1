#include "ExitButton.h"
#include "StateManager.h"
#include "States.h"
#include "Game.h"

ExitButton::ExitButton(SDL_Rect source, SDL_FRect destination, const char* textureKey)
	: UI_Button(source, destination, textureKey)
{

}

void ExitButton::Execute()
{
	StateManager::ChangeState(new EndState());
}