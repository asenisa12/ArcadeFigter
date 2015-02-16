#include "MainGame.h"
#include <cpprest/json.h> 
#include <fstream>

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 640;
const int ENEMIES_LEVEL1 = 2;

std::list<GameCharacter*> charactersList;

SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

GameBase mainGame(SCREEN_HEIGHT,SCREEN_WIDTH);
//grid::SquareGrid levelgrid(SCREEN_WIDTH, SCREEN_HEIGHT);


bool started;



int main(int argc, char* argv[])
{
	if (!mainGame.init()){
		printf("Can't init!");
	}
	else
	{	
		GameStateMachine stateMachine(&mainGame);
		stateMachine.changeState(new MainMenu());
		started = false;
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