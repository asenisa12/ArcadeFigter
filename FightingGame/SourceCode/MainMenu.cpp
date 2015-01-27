#include "MainMenu.h"


MainMenu::~MainMenu()
{
	delete backGroundMenu;
	delete startButton;
	delete exitButton;
}

void MainMenu::update(GameStateMachine *stateMachine)
{
	if (startButton->isPressed(&mainGame->gameEvent))
		stateMachine->pushState(new GameLevel());

	if (!mainGame->quit)
		mainGame->quit = exitButton->isPressed(&mainGame->gameEvent);

	return;
}

void MainMenu::render(SDL_Renderer* renderer)
{

	SDL_Rect camera = {0, 0, mainGame->getScreenW(), mainGame->getScreenH()};
	backGroundMenu->renderBack(&camera, renderer);
	startButton->renderButton(renderer);
	exitButton->renderButton(renderer);
}

bool  MainMenu::loadObjects()
{
	if (!backGroundMenu->loadMedia(mainGame->getRenderer())) return false;
	if (!startButton->loadMedia("Textures/StartButton.png", mainGame->getRenderer())) return false;
	if (!exitButton->loadMedia("Textures/ExitButton.png", mainGame->getRenderer())) return false;
	return true;
}

bool MainMenu::onEnter(GameBase *mainGame_)
{
	mainGame = mainGame_;
	int SCREEN_WIDTH = mainGame->getScreenW(), SCREEN_HEIGHT = mainGame->getScreenH();
	
	backGroundMenu = new BackGround("Textures/MenuBackground.png");
	startButton = new GameButton(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH / 5, SCREEN_HEIGHT / 5);
	exitButton = new GameButton(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH / 5, SCREEN_HEIGHT / 1.5);

	return loadObjects();
}

bool MainMenu::onExit()
{
	printf("exiting MainMenu state\n");
	return true;
}