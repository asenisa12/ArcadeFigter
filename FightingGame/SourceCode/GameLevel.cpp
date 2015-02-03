#include "GameLevel.h"

const std::string GameLevel::levelID = "GameLevel";


void GameLevel::update(GameStateMachine *stateMachine)
	
{
	charactersList.sort([](GameCharacter* struct1, GameCharacter* struct2)
		{return (struct1->getBottomY()< struct2->getBottomY()); });

	for (auto character : charactersList)
		character->doActions(&camera, charactersList);
}

void GameLevel::render(SDL_Renderer* renderer)
{

	std::list<GameCharacter*>::iterator it = charactersList.begin();
	backGroundLevel1->renderBack(&camera, mainGame->getRenderer());
	while (it != charactersList.end())
	{
		if ((*it)->getHealth() == 0)
		{
			it = charactersList.erase(it);
		}
		else
		{
			(*it)->renderCharacter(mainGame->getRenderer());
			++it;
		}
	}
	drawPlayerHealthBar(players.back()->getHealth());
}

void GameLevel::drawPlayerHealthBar(int health)
{
	int healthBarW = mainGame->getScreenW()*0.0064*(players.back()->getHealth() / 3);
	SDL_Rect healthbar = { 5, 5, healthBarW, mainGame->getScreenH()*0.05 };
	SDL_SetRenderDrawColor(mainGame->getRenderer(), 0xFF, 0, 0, 0);
	SDL_RenderFillRect(mainGame->getRenderer(), &healthbar);
	SDL_RenderDrawRect(mainGame->getRenderer(), &healthbar);
}

bool  GameLevel::LoadObjects(){
	if (!backGroundLevel1->loadMedia(mainGame->getRenderer())) return false;
	for (auto it : charactersList)
		if (!it->loadMedia(mainGame->getRenderer())) return false;
}

bool GameLevel::onEnter(GameBase *mainGame_)
{
	mainGame = mainGame_;
	camera = { 0, 0, mainGame->getScreenW(), mainGame->getScreenH()};
	backGroundLevel1 = new BackGround("Textures/Level1.png");
	levelgrid = new SquareGrid(mainGame->getScreenW(), mainGame->getScreenH());


	players.push_back(new Player("Resources/player.json", mainGame->getScreenW(), mainGame->getScreenH(), levelgrid, Player1));
	charactersList.push_back(new Enemy1("Textures/Enemy1.png", { 380, 370 }, mainGame->getScreenW(), mainGame->getScreenH(), players.back(), levelgrid));
	charactersList.push_back(new Enemy1("Textures/Enemy1.png", { 500, 370 }, mainGame->getScreenW(), mainGame->getScreenH(), players.back(), levelgrid));
	charactersList.push_back(players.back());

	
	return LoadObjects();
}

bool GameLevel::onExit()
{
	return true;
}

GameLevel::~GameLevel()
{
	delete backGroundLevel1;
	delete levelgrid;
}