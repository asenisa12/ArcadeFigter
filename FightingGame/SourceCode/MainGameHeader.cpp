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
		for (std::vector<GameCharacter*>::iterator it = charactersVect.begin(); it != charactersVect.end(); ++it)
		{
			(*it)->doActions(&camera, charactersVect);
		}
		std::sort(charactersVect.begin(), charactersVect.end(), greater_than());
	}
}

void render()
{
	SDL_SetRenderDrawColor(mainGame.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(mainGame.getRenderer());
	if (started){
		backGroundLevel1.renderBack(&camera, player1.getX(), mainGame.getRenderer());
		for (std::vector<GameCharacter*>::iterator it = charactersVect.begin(); it != charactersVect.end(); ++it)
		{
			if ((*it)->getHealth() > 0)
			{
				(*it)->renderCharacter(mainGame.getRenderer());
			}
			else
			{
				it = charactersVect.erase(it);
				it--;
			}
		}
		drawPlayerHealthBar(player1.getHealth());
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


	charactersVect.push_back(&enemy1);
	charactersVect.push_back(&enemy2);
	charactersVect.push_back(&player1);

	if (!player1.loadMedia(mainGame.getRenderer())) return false;
	if (!enemy1.loadMedia(mainGame.getRenderer())) return false;
	if (!enemy2.loadMedia(mainGame.getRenderer())) return false;
	if (!backGroundLevel1.loadMedia(mainGame.getRenderer())) return false;
	if (!backGroundMenu.loadMedia(mainGame.getRenderer())) return false;
	if (!startButton.loadMedia("Textures/StartButton.png", mainGame.getRenderer())) return false;
	if (!exitButton.loadMedia("Textures/ExitButton.png", mainGame.getRenderer())) return false;

	return true;
}

void drawPlayerHealthBar(int health)
{
	int healthBarW = SCREEN_WIDTH*0.0064*(player1.getHealth()/3);
	SDL_Rect healthbar = {5, 5, healthBarW,SCREEN_HEIGHT*0.05 };
	SDL_SetRenderDrawColor(mainGame.getRenderer(), 0xFF, 0, 0, 0);
	SDL_RenderFillRect(mainGame.getRenderer(), &healthbar);
	SDL_RenderDrawRect(mainGame.getRenderer(), &healthbar);
}