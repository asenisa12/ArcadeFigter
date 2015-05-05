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
	
	button[Plus]->renderButton(mainGame->getRenderer());
	button[Minus]->renderButton(mainGame->getRenderer());
	volumeBar->drawBar(mainGame->getRenderer());
}

void SettingsMenu::update()
{
	if (dataSettings["fullScreen"].GetBool())
	{
		if (button[Windowed]->isPressed(&mainGame->gameEvent))
		{
			SDL_SetWindowFullscreen(mainGame->getWindow(), 0);
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
	
	if (button[Plus]->isPressed(&mainGame->gameEvent))
	{
		volumeBar->incrVal();
		Mix_Volume(-1, 0);
	}
	if (button[Minus]->isPressed(&mainGame->gameEvent))
	{
		volumeBar->reduceVal();
		Mix_Volume(-1, 0);
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

	int barX = button[Plus]->getButtonRect().x;
	int barY = button[Plus]->getButtonRect().y - button[Plus]->getButtonRect().h * 1.5;
	int barW = button[Minus]->getButtonRect().x + button[Minus]->getButtonRect().w + 20 - button[Plus]->getButtonRect().x;
	int barH = mainGame_->getScreenH()/25;
	
	volumeBar = new VolumeBar(barX, barY, barW, barH, 
		dataSettings["volume"].GetInt(), mainGame_->getRenderer());

	volume = dataSettings["volume"].GetInt();
}
SettingsMenu::~SettingsMenu()
{
	delete volumeBar;
}