#include "GameState.h"


void GameStateMachine::changeState(GameState *newState)
{
	if (!states.empty())
	{
		if (states.back()->getStateID() == newState->getStateID())
		{
			return;
		}

		if (states.back()->onExit())
		{
			delete states.back();
			states.pop_back();
		}
	}

	states.push_back(newState);
	states.back()->onEnter(mainGame);

}
void  GameStateMachine::update()
{
	states.back()->update(this);
}
void GameStateMachine::render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(mainGame->getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(mainGame->getRenderer());
	states.back()->render(renderer);
	SDL_RenderPresent(mainGame->getRenderer());
}

void GameStateMachine::pushState(GameState *newState)
{
	states.push_back(newState);
	states.back()->onEnter(mainGame);
}

void GameStateMachine::popState()
{
	if (!states.empty())
	{
		if (states.back()->onExit())
		{
			delete states.back();
			states.pop_back();
		}
	}
}

GameStateMachine::GameStateMachine(GameBase* mainGame_)
	:mainGame(mainGame_)
{}

GameBase* GameStateMachine::getGameBase()
{
	return mainGame;
}