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
		BackGround backGround("Textures/texture.png", mainGame.getRenderer());
		Player player1("Textures/Ferris2.png", mainGame.getRenderer());

		if (!player1.loadMedia() && !backGround.loadMedia())
		{
			printf("Can't load media!");
		}
		else
		{
			SDL_Rect camera;
			camera.w = 640;
			camera.h = 480;
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
				camera.y = (player1.getY() + 92) / 2 - 480 / 2;
				camera.x = (player1.getX()+75)/2- 640/2;
				backGround.renderBack(&camera);
				//Update screen
				SDL_RenderPresent(mainGame.getRenderer());
			}
		}
	}
	system("pause");
	return 0;
}