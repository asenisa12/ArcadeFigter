#include "SettingsMenu.h"

bool SettingsMenu::loadMedia(){
	return true;
}

void SettingsMenu::render(){

}
void SettingsMenu::update()
{

}
SettingsMenu::SettingsMenu(std::string path, GameBase *mainGame_)
	:mainGame(mainGame_)
{
	dataFile.open(path);
	Document data = parse_jsonFile(path);

}
SettingsMenu::~SettingsMenu()
{
	dataFile.close();
}