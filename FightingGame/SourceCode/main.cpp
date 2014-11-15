#include <iostream>
#include <SDL.h>
#include "Player.h"
#include "GameBase.h"
#include "GameObject.h"
#include "BackGround.h"

static const int SCREEN_WIDTH = 640;
static const int SCREEN_HEIGHT = 480;


GameBase mainGame(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char* argv[])
{
	if (!mainGame.init()){
		printf("Can't init!");
	}
	else
	{	
		BackGround backGround("Textures/Level1.png", mainGame.getRenderer());
		Player player1("Textures/Mustafa1.png", mainGame.getRenderer(),SCREEN_WIDTH, SCREEN_HEIGHT);

		if (!player1.loadMedia() || !backGround.loadMedia())
		{
			printf("Can't load media!");
		}
		else
		{
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
			SDL_Event e;
			bool quit = false;

			while (!quit)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
				}
				SDL_SetRenderDrawColor(mainGame.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(mainGame.getRenderer());
				
				player1.doActions(e, &camera);
				
				backGround.renderBack(&camera, player1.getX());
				player1.renderPlayer();
				
				//Update screen
				SDL_RenderPresent(mainGame.getRenderer());
			}
		}
	}
	//system("pause");
	return 0;
}