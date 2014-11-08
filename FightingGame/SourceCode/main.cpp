#include <iostream>
#include "Player.h"
#include "GameBase.h"
#include <SDL.h>

GameBase mainGame(640, 480);

int main(int argc, char* argv[])
{

	if (!mainGame.init()){

	}
	else
	{	
		Player player1("Textures/Ferris2.png", mainGame.getRenderer());

		if (!player1.loadMedia())
		{

		}
		else
		{
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
				player1.doActions(e);
			}
		}
	}
	system("pause");
	return 0;
}