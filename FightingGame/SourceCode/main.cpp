#include "MainGame.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int ENEMIES_LEVEL1 = 2;

std::list<GameCharacter*> charactersList;

SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

GameBase mainGame(SCREEN_WIDTH, SCREEN_HEIGHT);
grid::SquareGrid levelgrid(SCREEN_WIDTH, SCREEN_HEIGHT);

BackGround backGroundLevel1("Textures/Level1.png");
BackGround backGroundMenu("Textures/MenuBackground.png");
Player player1("Textures/Mustafa1.png", SCREEN_WIDTH, SCREEN_HEIGHT, {60, 410}, &levelgrid);
Enemy1 enemy1("Textures/Enemy1.png", {380,370}, SCREEN_WIDTH, SCREEN_HEIGHT, &player1, &levelgrid);
Enemy1 enemy2("Textures/Enemy1.png", { 500, 370 }, SCREEN_WIDTH, SCREEN_HEIGHT, &player1, &levelgrid);
GameButton startButton(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH / 5, SCREEN_HEIGHT/5);
GameButton exitButton(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH/5, SCREEN_HEIGHT/1.5);

int gameState = GameState::MainMenu;
bool started;



int main(int argc, char* argv[])
{
	if (!mainGame.init()){
		printf("Can't init!");
	}
	else
	{	
		if (!loadMedia())
		{
			printf("Can't load media!");
		}
		else
		{
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
				update();

				//render
				render();
			}
		}
	}
	//system("pause");
	return 0;
}