#include "GameHeaders.h"

static const int SCREEN_WIDTH = 640;
static const int SCREEN_HEIGHT = 480;

bool started;
SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
SDL_Event gameEvent;

GameBase mainGame(SCREEN_WIDTH, SCREEN_HEIGHT);
BackGround backGroundLevel1("Textures/Level1.png");
Player player1("Textures/Mustafa1.png",SCREEN_WIDTH, SCREEN_HEIGHT);
GameButton startButton(SCREEN_WIDTH, SCREEN_HEIGHT);

void update()
{
	int a = SCREEN_WIDTH;
	player1.doActions(gameEvent, &camera);
	if (!started)
	{
		started = startButton.isPressed(&gameEvent);
	}
}

void render()
{
	SDL_SetRenderDrawColor(mainGame.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(mainGame.getRenderer());

	if (started){
		backGroundLevel1.renderBack(&camera, player1.getX(), mainGame.getRenderer());
		player1.renderPlayer(mainGame.getRenderer());
	}
	else
	{
		startButton.renderButton(mainGame.getRenderer());
	}

	SDL_RenderPresent(mainGame.getRenderer());
}

void getInput();

int main(int argc, char* argv[])
{
	if (!mainGame.init()){
		printf("Can't init!");
	}
	else
	{	

		
		if (!player1.loadMedia(mainGame.getRenderer()) || !backGroundLevel1.loadMedia(mainGame.getRenderer()) ||
			!startButton.loadMedia("Textures/buttons.png",mainGame.getRenderer()))
		{
			printf("Can't load media!");
		}
		else
		{
			
			started = false;
			bool quit = false;

			//game loop
			while (!quit)
			{
				while (SDL_PollEvent(&gameEvent) != 0)
				{
					if (gameEvent.type == SDL_QUIT)
					{
						quit = true;
					}
				}
				//update
				update();

				//render
				render();
			}
		}
	}
	system("pause");
	return 0;
}