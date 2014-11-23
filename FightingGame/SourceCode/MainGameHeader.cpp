#include "MainGameHeader.h"

void update()
{
	int a = SCREEN_WIDTH;
	player1.doActions(gameEvent, &camera);
	if (!started)
	{
		started = startButton.isPressed(&gameEvent);
		if (!quit)
			quit = exitButton.isPressed(&gameEvent);
	}
}

void render()
{
	SDL_SetRenderDrawColor(mainGame.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(mainGame.getRenderer());

	if (started){
		backGroundLevel1.renderBack(&camera, player1.getX(), mainGame.getRenderer());
		player1.renderPlayer(mainGame.getRenderer());
	}
	else
	{
		backGroundMenu.renderBack(&camera, 0, mainGame.getRenderer());
		startButton.renderButton(mainGame.getRenderer());
		exitButton.renderButton(mainGame.getRenderer());
	}

	SDL_RenderPresent(mainGame.getRenderer());
}

bool loadMedia()
{
	if (!player1.loadMedia(mainGame.getRenderer())) return false;
	if (!backGroundLevel1.loadMedia(mainGame.getRenderer())) return false;
	if (!backGroundMenu.loadMedia(mainGame.getRenderer())) return false;
	if (!startButton.loadMedia("Textures/StartButton.png", mainGame.getRenderer())) return false;
	if (!exitButton.loadMedia("Textures/ExitButton.png", mainGame.getRenderer())) return false;

	return true;
}