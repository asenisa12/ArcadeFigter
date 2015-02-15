#include "MainMenu.h"

void MainMenu::update(GameStateMachine *stateMachine)
{
			

	switch (currentMenu)
	{
	case Menu1:
		if (button[Start]->isPressed(&mainGame->gameEvent))
			currentMenu = Menu2;
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
	}

	if (!mainGame->quit)
		mainGame->quit = button[Exit]->isPressed(&mainGame->gameEvent);

	return;
}

void MainMenu::handleEvent()
{

	switch (mainGame->gameEvent.type)
	{
	case SDL_KEYUP:
		switch (mainGame->gameEvent.key.keysym.sym){
		case SDLK_ESCAPE:
			mainGame->quit = true;
			break;
		default:
			break;
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
		break;
	case Menu2:
		button[p2Mode]->renderButton(renderer);
		button[p1Mode]->renderButton(renderer);
		break;
	case Menu3:
		button[Level1]->renderButton(renderer);
		button[Level2]->renderButton(renderer);
		break;
	}
	button[Exit]->renderButton(renderer);
}

bool  MainMenu::loadObjects()
{
	if (!backGroundMenu->loadMedia(mainGame->getRenderer())) return false;
	for (auto const it : button)
		if (!(it.second)->loadMedia(mainGame->getRenderer())) return false;

	return true;
}

bool MainMenu::onEnter(GameBase *mainGame_)
{
	mainGame = mainGame_;
	int w = mainGame->getScreenW(), h = mainGame->getScreenH();
	
	std::fstream jsonFile;
	jsonFile.open(menuData);
	if (jsonFile.is_open())
	{
		jsonObj data = jsonObj::parse(jsonFile);

		backGroundMenu = new BackGround(utility::conversions::to_utf8string(
			data[U("backgroundTexture")].as_string()));

		web::json::array buttons = data[U("buttons")].as_array();

		for (int i = 0; i < buttons.size(); i++)
			button[i] = new GameButton(w, h, buttons.at(i));
	}
	else
	{
		printf("Error: Can't open file %s\n", menuData.c_str());
	}

	currentMenu = Menu1;
	return loadObjects();
}

bool MainMenu::onExit()
{
	printf("exiting MainMenu state\n");
	return true;
}