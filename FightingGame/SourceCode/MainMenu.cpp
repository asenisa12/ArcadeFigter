#include "MainMenu.h"

const std::string MainMenu::menuID = "MainMenu";

void MainMenu::update(GameStateMachine *stateMachine)
{
	if (startButton.isPressed(&mainGame->gameEvent))
		stateMachine->pushState(new GameLevel());

	if (!mainGame->quit)
		mainGame->quit = exitButton.isPressed(&mainGame->gameEvent);
}

void MainMenu::render(SDL_Renderer* renderer)
{

}

bool MainMenu::onEnter()
{
	int SCREEN_WIDTH = mainGame->getScreenW(), SCREEN_HEIGHT = mainGame->getScreenH();
	
	backGroundMenu = BackGround("Textures/MenuBackground.png");
	startButton = GameButton(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH / 5, SCREEN_HEIGHT / 5);
	exitButton = GameButton(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH / 5, SCREEN_HEIGHT / 1.5);
}

bool MainMenu::onExit()
{
	printf("exiting MainMenu state\n");
}