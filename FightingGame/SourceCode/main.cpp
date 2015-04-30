#include "GameBase.h"
#include "GameState.h"
#include "MainMenu.h"

using namespace std;
using namespace rapidjson;

const int FPS = 60;
const int DELAY_TIME = 1000.0f / FPS;

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

GameBase mainGame(SCREEN_HEIGHT,SCREEN_WIDTH);


int main(int argc, char* argv[])
{

	Uint32 frameStart, frameTime;
	if (!mainGame.init()){
		printf("Can't init!");
	}
	else
	{	
		GameStateMachine stateMachine(&mainGame);
		stateMachine.changeState(new MainMenu());
		mainGame.quit = false;

		//game loop
		while (!mainGame.quit)
		{
			while (SDL_PollEvent(&mainGame.gameEvent) != 0)
			{
				if (mainGame.gameEvent.type == SDL_QUIT)
				{
					mainGame.quit = true;
				}
			}
			frameStart = SDL_GetTicks();
			stateMachine.handleEvent();
			
			//update
			stateMachine.update();

			//render
			stateMachine.render(mainGame.getRenderer());

			frameTime = SDL_GetTicks() - frameStart;
			if (frameTime< DELAY_TIME)
			{
				SDL_Delay((int)(DELAY_TIME - frameTime));
			}
		}
	}
	return 0;
}