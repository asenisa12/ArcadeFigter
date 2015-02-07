#include "GameLevel.h"

GameLevel::GameLevel(bool gameMode1_, bool level1_)
	: gameMode1(gameMode1_), level1(level1_)
{}

const std::string GameLevel::levelID = "GameLevel";

const pKeys GameLevel::player1Keys = 
{ SDL_SCANCODE_D, SDL_SCANCODE_A, SDL_SCANCODE_W, SDL_SCANCODE_S, 
SDL_SCANCODE_V, SDL_SCANCODE_B, SDL_SCANCODE_N };
const pKeys GameLevel::player2Keys = 
{ SDL_SCANCODE_RIGHT, SDL_SCANCODE_LEFT, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN,
SDL_SCANCODE_RCTRL, SDL_SCANCODE_RSHIFT, SDL_SCANCODE_RALT };


void GameLevel::update(GameStateMachine *stateMachine)
	
{
	players.front()->handleEvent(player1Keys);
	players.back()->handleEvent(player2Keys);

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

bool GameLevel::createLevel()
{
	return true;
}

bool GameLevel::onEnter(GameBase *mainGame_)
{
	mainGame = mainGame_;
	camera = { 0, 0, mainGame->getScreenW(), mainGame->getScreenH()};
	if (level1){
		backGroundLevel1 = new BackGround("Textures/Level1.png");
	}
	else
	{
		backGroundLevel1 = new BackGround("Textures/Level2.png");
	}
	levelgrid = new SquareGrid(mainGame->getScreenW(), mainGame->getScreenH());


	players.push_back(new Player("Resources/player.json", mainGame->getScreenW(), mainGame->getScreenH(), levelgrid, Player1));
	if (gameMode1)
		players.push_back(new Player("Resources/player.json", mainGame->getScreenW(), mainGame->getScreenH(), levelgrid, Player2));
	charactersList.push_back(new Enemy1("Resources/enemy.json", { 380, 370 }, mainGame->getScreenW(), mainGame->getScreenH(), players.back(), levelgrid, FERRIS));
	charactersList.push_back(new Enemy1("Resources/enemy.json", { 500, 370 }, mainGame->getScreenW(), mainGame->getScreenH(), players.front(), levelgrid, FERRIS));
	charactersList.push_back(players.back());
	charactersList.push_back(players.front());

	
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