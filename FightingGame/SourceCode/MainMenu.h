#pragma once

#include <string>
#include <fstream>
#include <map>
#include "GameState.h"
#include "BackGround.h"
#include "GameButton.h"
#include "GameLevel.h"
#include "GameLabel.h"
#include "JsonReader.h"
#include "SettingsMenu.h"

static const std::string menuData ="Resources/menuData.json";

class MainMenu : public GameState
{
	SettingsMenu* settingsMenu;
	int backLabeY, backLabeX;
	GameLabel* back;
	enum{Menu1=0, Menu2=1, Menu3=2, Settings=3};
	bool esckapePressed;
	int gameMode, level;
	int currentMenu;
	BackGround *backGroundMenu;
	std::map<int, GameButton*> button;
	bool loadObjects();
public:
	virtual void handleEvent();
	virtual void update(GameStateMachine *stateMachine);
	virtual void render(SDL_Renderer* renderer);
	virtual bool onEnter(GameBase *mainGame_);
	virtual bool onExit();
	virtual std::string getStateID() const { return "MainMenu"; }
};