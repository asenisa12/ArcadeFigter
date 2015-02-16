#include "GameBase.h"
#include "GameState.h"
#include "MainMenu.h"

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

GameBase mainGame(SCREEN_HEIGHT,SCREEN_WIDTH);


int main(int argc, char* argv[])
{
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
			stateMachine.handleEvent();

			//update
			stateMachine.update();

			//render
			stateMachine.render(mainGame.getRenderer());
		}
	}
	//system("pause");
	return 0;
}