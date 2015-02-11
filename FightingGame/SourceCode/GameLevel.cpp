#include "GameLevel.h"

std::vector < std::tuple<GameCharacter*, bool> > players;

GameLevel::GameLevel(int gameMode_, int level_)
	: gameMode(gameMode_), level(level_)
{}

const std::string GameLevel::levelID = "GameLevel";

const pKeys GameLevel::player1Keys = 
{ SDL_SCANCODE_D, SDL_SCANCODE_A, SDL_SCANCODE_W, SDL_SCANCODE_S, 
SDL_SCANCODE_V, SDL_SCANCODE_B, SDL_SCANCODE_N };
const pKeys GameLevel::player2Keys = 
{ SDL_SCANCODE_RIGHT, SDL_SCANCODE_LEFT, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN,
SDL_SCANCODE_RCTRL, SDL_SCANCODE_RSHIFT, SDL_SCANCODE_RALT };


void GameLevel::handleEvent()
{
	players_.front()->handleEvent(player1Keys);
	
	manage_camera();
	if (gameMode == p2Mode)
		players_.back()->handleEvent(player2Keys);
}

void GameLevel::update(GameStateMachine *stateMachine)
	
{

	charactersList.sort([](GameCharacter* struct1, GameCharacter* struct2)
		{return (struct1->getBottomY()< struct2->getBottomY()); });

	for (auto character : charactersList)
		character->update(charactersList);
}

void GameLevel::render(SDL_Renderer* renderer)
{

	std::list<GameCharacter*>::iterator it = charactersList.begin();
	backGroundLevel1->renderBack(&camera, mainGame->getRenderer());
	while (it != charactersList.end())
	{
		if ((*it)->ready_for_delete())
		{
			it = charactersList.erase(it);
		}
		else
		{
			(*it)->renderCharacter(mainGame->getRenderer());
			++it;
		}
	}
	drawPlayerHealthBar(players_.back()->getHealth());
}

void GameLevel::drawPlayerHealthBar(int health)
{
	int healthBarW = mainGame->getScreenW()*0.0064*(players_.back()->getHealth() / 3);
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

void GameLevel::manage_camera()
{
	if (cameraPosCount < CAMERA_POSITIONS)
	{
		int playersAtEndOfCAmera = 0;
		for (auto player : players_)
		{
			//last x position on screen = 90% from screen W 
			if (player->getX() > mainGame->getScreenW()*(0.90))
			{
				playersAtEndOfCAmera++;
			}
		}
		if (playersAtEndOfCAmera == players.size()){
			for (auto player : players_) player->manageCameraPos();
			camera.x += mainGame->getScreenW();
			cameraPosCount++;
		}
	}

}

bool GameLevel::createLevel()
{
	return true;
}

bool GameLevel::onEnter(GameBase *mainGame_)
{
	cameraPosCount = 0;
	mainGame = mainGame_;
	camera = { 0, 0, mainGame->getScreenW(), mainGame->getScreenH()};
	if (level==Level1){
		backGroundLevel1 = new BackGround("Textures/Level1.png");
	}
	else if (level == Level2)
	{
		backGroundLevel1 = new BackGround("Textures/Level2.png");
	}
	levelgrid = new SquareGrid(mainGame->getScreenW(), mainGame->getScreenH());


	players_.push_back(new Player("Resources/player.json", mainGame->getScreenW(), mainGame->getScreenH(), levelgrid, Player1));
	players.push_back(std::make_tuple(players_.front(),true));
	charactersList.push_back(players_.front());
	if (gameMode == p2Mode)
	{
		players_.push_back(new Player("Resources/player.json", mainGame->getScreenW(), mainGame->getScreenH(), levelgrid, Player2));
		charactersList.push_back(players_.back());
		players.push_back(std::make_tuple(players_.back(), true));
	}
	charactersList.push_back(new Enemy1("Resources/enemy.json", { 380, 350 }, mainGame->getScreenW(), mainGame->getScreenH(), levelgrid, FERRIS));
	charactersList.push_back(new Enemy1("Resources/enemy.json", { 500, 370 }, mainGame->getScreenW(), mainGame->getScreenH(), levelgrid, FERRIS));
	charactersList.push_back(new Enemy1("Resources/enemy.json", { 560, 370 }, mainGame->getScreenW(), mainGame->getScreenH(), levelgrid, FERRIS));
	charactersList.push_back(new Enemy1("Resources/enemy.json", { 300, 370 }, mainGame->getScreenW(), mainGame->getScreenH(), levelgrid, FERRIS));
	

	
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