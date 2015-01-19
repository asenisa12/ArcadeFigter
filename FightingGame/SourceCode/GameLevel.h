#pragma once

#include "GameState.h"

class GameLevel : public GameState
{
	static const std::string levelID;
public:
	virtual void update(GameStateMachine *stateMachine);
	virtual void render(SDL_Renderer* renderer);
	virtual bool onEnter();
	virtual bool onExit();
	virtual std::string getStateID() const { return levelID; }
};