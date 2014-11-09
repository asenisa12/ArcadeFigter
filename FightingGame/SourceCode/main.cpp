#include <iostream>
#include "Player.h"
#include "GameBase.h"
#include <SDL.h>
#include "GameObject.h"
#include "BackGround.h"

GameBase mainGame(640, 480);

int main(int argc, char* argv[])
{
	if (!mainGame.init()){
		printf("Can't init!");
	}
	else
	{	
		BackGround backGround("Textures/Level1.png", mainGame.getRenderer());
		Player player1("Textures/Ferris2.png", mainGame.getRenderer());

		if (!player1.loadMedia() || !backGround.loadMedia())
		{
			printf("Can't load media!");
		}
		else
		{
			SDL_Rect camera = { 0, 0, 640, 480 };
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
				
				player1.doActions(e);
				camera.x = (player1.getX())-640/2;

				backGround.renderBack(&camera);
				player1.renderPlayer();
				
				//Update screen
				SDL_RenderPresent(mainGame.getRenderer());
			}
		}
	}
	//system("pause");
	return 0;
}