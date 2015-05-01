#include "SettingsMenu.h"

bool SettingsMenu::loadMedia(){
	for (auto but : button)
		if (!but->loadMedia(mainGame->getRenderer())) return false;

	return true;
}

void SettingsMenu::render(){
	if (dataSettings["fullScreen"].GetBool())
		button[Windowed]->renderButton(mainGame->getRenderer());
	else button[FullScr]->renderButton(mainGame->getRenderer());

}

void SettingsMenu::update()
{
	if (dataSettings["fullScreen"].GetBool())
	{
		if (button[Windowed]->isPressed(&mainGame->gameEvent))
		{
			SDL_SetWindowFullscreen(mainGame->getWindow(), SDL_WINDOW_SHOWN);
			dataSettings["fullScreen"].SetBool(false);
			write_json_to_File(filePath, dataSettings);
		}
	}
	else
	{
		if (button[FullScr]->isPressed(&mainGame->gameEvent))
		{
			SDL_SetWindowFullscreen(mainGame->getWindow(), SDL_WINDOW_FULLSCREEN);
			dataSettings["fullScreen"].SetBool(true);
			write_json_to_File(filePath, dataSettings);
		}
	}
}

SettingsMenu::SettingsMenu(std::string path, GameBase *mainGame_)
	:mainGame(mainGame_), dataSettings(parse_jsonFile(path)), filePath(path)
{
	Value& buttons = dataSettings["buttons"];
	int w = mainGame_->getScreenW(), h = mainGame_->getScreenH();
	
	assert(buttons.IsArray());
	for (SizeType i = 0; i < buttons.Size(); i++)
		button.push_back(new GameButton(w, h, buttons[i]));
}
SettingsMenu::~SettingsMenu()
{

}