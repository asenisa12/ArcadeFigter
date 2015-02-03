#include "MainGame.h"
#include <cpprest/json.h> 
#include <fstream>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int ENEMIES_LEVEL1 = 2;

std::list<GameCharacter*> charactersList;

SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

GameBase mainGame(SCREEN_HEIGHT,SCREEN_WIDTH);
grid::SquareGrid levelgrid(SCREEN_WIDTH, SCREEN_HEIGHT);

int gameState = GameState1::MainMenu1;
bool started;



int main(int argc, char* argv[])
{
	//fstream file;
	//file.open("Resources/player.json");
	/////*file.seekg(0, ios::end);
	////int size =  file.tellg();*/
	//web::json::value obj = web::json::value::parse(file);
	////obj[U("a")] = web::json::value::number(12);
	//cout << (obj[U("a")])[U("b")].as_integer() << endl;

	//file.close();
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
			//update
			stateMachine.update();

			//render
			stateMachine.render(mainGame.getRenderer());
		}
	}
	//system("pause");
	return 0;
}