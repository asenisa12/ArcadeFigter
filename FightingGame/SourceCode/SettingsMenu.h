#pragma once 

#include "GameButton.h"
#include "GameBase.h"
#include "JsonReader.h"

#include <string>
#include <fstream>
#include <vector>

enum{FullScr=0, Windowed=1};
class SettingsMenu
{
	std::vector<GameButton*> button;
	std::string filePath;
	Document dataSettings;
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