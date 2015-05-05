#include "MainMenu.h"

void MainMenu::update(GameStateMachine *stateMachine)
{
			

	switch (currentMenu)
	{
	case Menu1:
		if (button[Start]->isPressed(&mainGame->gameEvent))
			currentMenu = Menu2;
		if (button[settings]->isPressed(&mainGame->gameEvent))
			currentMenu = Settings;
		break;
	case Menu2:
		if (button[p1Mode]->isPressed(&mainGame->gameEvent))
		{
			gameMode = p1Mode;
			currentMenu = Menu3;
			
		}
		if (button[p2Mode]->isPressed(&mainGame->gameEvent))
		{
			gameMode = p2Mode;
			currentMenu = Menu3;
		}
		break;
	case Menu3:
		if (button[Level1]->isPressed(&mainGame->gameEvent))
			stateMachine->pushState(new GameLevel(gameMode, Level1));
		if (button[Level2]->isPressed(&mainGame->gameEvent))
			stateMachine->pushState(new GameLevel(gameMode, Level2));
		break;
	case Settings:
		settingsMenu->update();
		break;

	}

	if (!mainGame->quit && currentMenu!=Settings)
		mainGame->quit = button[Exit]->isPressed(&mainGame->gameEvent);

	return;
}

void MainMenu::handleEvent()
{

	switch (mainGame->gameEvent.type)
	{
	case SDL_KEYUP:
		if (mainGame->gameEvent.key.keysym.sym == SDLK_ESCAPE){
			esckapePressed = true;
		}
		break;
	case SDL_KEYDOWN:
		if (mainGame->gameEvent.key.keysym.sym == SDLK_ESCAPE &&
			esckapePressed){

			esckapePressed = false;

			if (currentMenu > 0 && currentMenu<3) currentMenu--;
			else if (currentMenu == 3) currentMenu = Menu1;
			else if (currentMenu == 0) mainGame->quit = true;
		}
		break;
	}
}

void MainMenu::render(SDL_Renderer* renderer)
{

	SDL_Rect camera = {0, 0, mainGame->getScreenW(), mainGame->getScreenH()};
	backGroundMenu->renderBack(&camera, renderer);
	

	switch (currentMenu)
	{
	case Menu1:
		button[Start]->renderButton(renderer);
		button[settings]->renderButton(renderer);
		break;
	case Menu2:
		button[p2Mode]->renderButton(renderer);
		button[p1Mode]->renderButton(renderer);
		break;
	case Menu3:
		button[Level1]->renderButton(renderer);
		button[Level2]->renderButton(renderer);
		break;
	case Settings:
		//back->renderLabel(backLabeX, backLabeY, renderer);
		settingsMenu->render();
		break;
	}
	if (currentMenu != Settings)
		button[Exit]->renderButton(renderer);
}

bool  MainMenu::loadObjects()
{
	if (!backGroundMenu->loadMedia(mainGame->getRenderer())) return false;
	if (!settingsMenu->loadMedia()) return false;
	if (!back->loadMedia(mainGame->getRenderer())) return false;
	for (auto const it : button)
		if (!(it.second)->loadMedia(mainGame->getRenderer())) return false;

	return true;
}

bool MainMenu::onEnter(GameBase *mainGame_)
{
	mainGame = mainGame_;
	esckapePressed = true;

	int w = mainGame->getScreenW(), h = mainGame->getScreenH();

	Document data = parse_jsonFile(menuData);

	backGroundMenu = new BackGround(data["backgroundTexture"].GetString());


	back = new GameLabel(data["backLabel"], mainGame_->getScreenW(), mainGame_->getScreenH());
	backLabeY = mainGame->getScreenH() / 2 - back->getH() / 2;
	backLabeX = mainGame->getScreenW() /3 - back->getW() / 2;

	Value& buttons = data["buttons"];
	
	settingsMenu = new SettingsMenu(data["settingsPath"].GetString(),mainGame_);

	assert(buttons.IsArray());
	for (SizeType i = 0; i < buttons.Size(); i++)
		button[i] = new GameButton(w, h, buttons[i]);


	currentMenu = Menu1;
	return loadObjects();
}

bool MainMenu::onExit()
{
	printf("exiting MainMenu state\n");
	delete backGroundMenu;
	delete settingsMenu;
	delete back;
	return true;
}