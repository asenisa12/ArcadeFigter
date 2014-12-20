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
		//player1.doActions(&camera, charactersList);
		charactersList.sort([](GameCharacter* struct1, GameCharacter* struct2)
			{return (struct1->getBottomY()< struct2->getBottomY()); });
	}
}

void render()
{
	SDL_SetRenderDrawColor(mainGame.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(mainGame.getRenderer());
	if (started){
		int i = 0;
		backGroundLevel1.renderBack(&camera, player1.getX(), mainGame.getRenderer());
		for (std::list<GameCharacter*>::iterator it = charactersList.begin(); it != charactersList.end(); ++it)
		{
			if ((*it)->getHealth() > 0)
			{
				(*it)->renderCharacter(mainGame.getRenderer());
				(*it)->doActions(&camera, charactersList);
			}
			else
			{
				it = charactersList.erase(it);
				if (i > 0)
				{
					it--;
				}
				else
				{
					it++;
				}
			}
			i++;
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
	charactersList.push_back(&enemy1);
	charactersList.push_back(&enemy2);
	charactersList.push_back(&player1);

	printf("----------\n");
	for (auto it : levelgrid.neighbors({ 600, 430, false }))
	{
		printf(", x:%d , Y:%d\n", it->X, it->Y);
	}

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