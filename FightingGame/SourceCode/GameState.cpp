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
	states.back()->onEnter();

}
void  GameStateMachine::update()
{
	states.back()->update(this);
}
void GameStateMachine::render(SDL_Renderer* renderer)
{
	states.back()->render(renderer);
}