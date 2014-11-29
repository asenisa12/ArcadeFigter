#include "MainGameHeader.h"

inline bool greater_than::operator() (GameCharacter* struct1, GameCharacter* struct2)
{
	return (struct1->getBottomY()< struct2->getBottomY());
}

void update()
{
	if (!started)
	{
		started = startButton.isPressed(&gameEvent);
		if (!quit)
			quit = exitButton.isPressed(&gameEvent);
	}
	else
	{
		enemy1.doActions(enemy);
		enemy2.doActions(enemy);
		player1.doActions(gameEvent, &camera, enemy);
		std::sort(characters.begin(), characters.end(), greater_than());
	}
}

void render()
{
	SDL_SetRenderDrawColor(mainGame.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(mainGame.getRenderer());
	if (started){
		backGroundLevel1.renderBack(&camera, player1.getX(), mainGame.getRenderer());
		for (std::vector<GameCharacter*>::iterator it = characters.begin(); it != characters.end(); ++it){
			//printf("Y:%d\n", (*it)->getBottomY());
			(*it)->renderCharacter(mainGame.getRenderer());
		}
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

	enemy[0] = &enemy1;
	enemy[1] = &enemy2;
	enemy[2] = &player1;
	for (int i = 0; i < 3; i++) 
		characters.push_back(enemy[i]);

	if (!player1.loadMedia(mainGame.getRenderer())) return false;
	if (!enemy1.loadMedia(mainGame.getRenderer())) return false;
	if (!enemy2.loadMedia(mainGame.getRenderer())) return false;
	if (!backGroundLevel1.loadMedia(mainGame.getRenderer())) return false;
	if (!backGroundMenu.loadMedia(mainGame.getRenderer())) return false;
	if (!startButton.loadMedia("Textures/StartButton.png", mainGame.getRenderer())) return false;
	if (!exitButton.loadMedia("Textures/ExitButton.png", mainGame.getRenderer())) return false;

	return true;
}