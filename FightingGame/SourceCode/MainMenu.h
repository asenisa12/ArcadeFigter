#pragma once

#include "GameState.h"
#include "BackGround.h"
#include "GameButton.h"
#include "GameLevel.h"

class MainMenu : public GameState
{
	BackGround backGroundMenu;
	GameButton startButton;
	GameButton exitButton;
	static const std::string menuID;
public:
	virtual void update(GameStateMachine *stateMachine);
	virtual void render(SDL_Renderer* renderer);
	virtual bool onEnter();
	virtual bool onExit();
	virtual std::string getStateID() const { return menuID; }
};