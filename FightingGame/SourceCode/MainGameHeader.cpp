#include "MainGameHeader.h"

inline bool greater_than::operator() (GameCharacter* struct1, GameCharacter* struct2)
{
	return (struct1->getBottomY()< struct2->getBottomY());
}



void update()
{
	switch (gameState)
	{
	case GameState::MainMenu:
		if (startButton.isPressed(&gameEvent))
			gameState = GameState::Level1;

		if (!quit)
			quit = exitButton.isPressed(&gameEvent);
		break;
	case GameState::Level1:
		charactersList.sort([](GameCharacter* struct1, GameCharacter* struct2)
		{return (struct1->getBottomY()< struct2->getBottomY()); });
		break;
	case GameState::Level2:
		break;
	}

}

void render()
{
	SDL_SetRenderDrawColor(mainGame.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(mainGame.getRenderer());

	switch (gameState)
	{
	case GameState::MainMenu:

		backGroundMenu.renderBack(&camera, 0, mainGame.getRenderer());

		startButton.renderButton(mainGame.getRenderer());

		exitButton.renderButton(mainGame.getRenderer());

		break;
	case GameState::Level1:

		backGroundLevel1.renderBack(&camera, player1.getX(), mainGame.getRenderer());
		for (auto it : charactersList)
		{
			if (it->getHealth() > 0)
			{
				it->renderCharacter(mainGame.getRenderer());
				it->doActions(&camera, charactersList);
			}
			else
			{
				charactersList.remove(it);
			}
		}
		drawPlayerHealthBar(player1.getHealth());

		break;
	case GameState::Level2:
		break;
	}

	SDL_RenderPresent(mainGame.getRenderer());
}

bool loadMedia()
{
	charactersList.push_back(&enemy1);
	charactersList.push_back(&enemy2);
	charactersList.push_back(&player1);
	std::unordered_map<Location, Location, LocationHash, Equal> came_from;
	std::unordered_map<Location, int, LocationHash, Equal> cost_so_far;
	/*path_search(levelgrid,{ 620, 430 }, { 20, 370 },came_from,cost_so_far);
	auto path = reconstruct_path({ 620, 430 }, { 20, 370 }, came_from);
	for (auto it : path)
	{
		printf("x-%d y-%d\n", it.X, it.Y);
		levelgrid.change_cost(it, 5);
	}*/
	if (!player1.loadMedia(mainGame.getRenderer())) return false;
	if (!enemy1.loadMedia(mainGame.getRenderer())) return false;
	if (!enemy2.loadMedia(mainGame.getRenderer())) return false;
	if (!backGroundLevel1.loadMedia(mainGame.getRenderer())) return false;
	if (!backGroundMenu.loadMedia(mainGame.getRenderer())) return false;
	if (!startButton.loadMedia("Textures/StartButton.png", mainGame.getRenderer())) return false;
	if (!exitButton.loadMedia("Textures/ExitButton.png", mainGame.getRenderer())) return false;

	return true;
}

void drawPlayerHealthBar(int health)
{
	int healthBarW = SCREEN_WIDTH*0.0064*(player1.getHealth()/3);
	SDL_Rect healthbar = {5, 5, healthBarW,SCREEN_HEIGHT*0.05 };
	SDL_SetRenderDrawColor(mainGame.getRenderer(), 0xFF, 0, 0, 0);
	SDL_RenderFillRect(mainGame.getRenderer(), &healthbar);
	SDL_RenderDrawRect(mainGame.getRenderer(), &healthbar);
}