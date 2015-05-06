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
		if (volume < MIX_MAX_VOLUME) volume += MIX_MAX_VOLUME / 8;
		if (volume > MIX_MAX_VOLUME) volume = MIX_MAX_VOLUME;
		dataSettings["volume"].SetInt(volume);
		write_json_to_File(filePath, dataSettings);
	}
	if (button[Minus]->isPressed(&mainGame->gameEvent))
	{
		volumeBar->reduceVal();
		if (volume > 0) volume -= MIX_MAX_VOLUME / 8;
		if (volume < 0) volume = 0;
		dataSettings["volume"].SetInt(volume);
		write_json_to_File(filePath, dataSettings);
	}
	
	Mix_VolumeMusic(volume);
	Mix_Volume(-1,volume);
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
	
	volume = dataSettings["volume"].GetInt();
	volumeBar = new VolumeBar(barX, barY, barW, barH, 
		&volume, mainGame_->getRenderer());

}
SettingsMenu::~SettingsMenu()
{
	delete volumeBar;
}