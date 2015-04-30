#pragma once 

#include "GameButton.h"
#include "GameBase.h"
#include "JsonReader.h"

#include <string>
#include <fstream>


class SettingsMenu
{
	std::fstream dataFile;
	GameBase* mainGame;
	GameButton* fScr_button;
	GameButton* windo_button;
public:
	bool loadMedia();
	void render();
	void update();
	SettingsMenu(std::string path, GameBase *mainGame_);
	~SettingsMenu();
};