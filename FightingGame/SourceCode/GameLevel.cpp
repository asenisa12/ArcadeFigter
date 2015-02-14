#include "GameLevel.h"

std::vector < std::tuple<GameCharacter*, bool> > players;

GameLevel::GameLevel(int gameMode_, int level_)
	: gameMode(gameMode_), level(level_)
{}

const std::string GameLevel::levelID = "GameLevel";

const pKeys GameLevel::player1Keys = 
{ SDL_SCANCODE_D, SDL_SCANCODE_A, SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_G,
SDL_SCANCODE_V, SDL_SCANCODE_B, SDL_SCANCODE_N };
const pKeys GameLevel::player2Keys = 
{ SDL_SCANCODE_RIGHT, SDL_SCANCODE_LEFT, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN,
SDL_SCANCODE_RETURN, SDL_SCANCODE_RCTRL, SDL_SCANCODE_RSHIFT, SDL_SCANCODE_RALT };


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
			charactersList.splice(charactersList.end(), enemies[cameraPosCount]);
		}
	}

}

GameCharacter* GameLevel::get_enemy(jsonObj data)
{
	int row = data[U("row")].as_integer();
	int col = data[U("col")].as_integer();
	int type = data[U("type")].as_integer();

	Location position = levelgrid->getLocation(row, col);
	int w = mainGame->getScreenW();
	int h = mainGame->getScreenH();
	Enemy1 enemy("Resources/enemy.json", position, w, h, levelgrid, type);
	return &enemy;
}

bool GameLevel::createLevel()
{
	std::fstream levelData;
	levelData.open("Resources/level.json");
	if (levelData.is_open())
	{
		jsonObj file = jsonObj::parse(levelData);
		jsonObj level;
		if (level == Level1)
		{
		}
		else if (level == Level2)
		{
			level = file[U("Level2")];
		}
			level = file[U("Level1")];
		textureBgraund = utility::conversions::to_utf8string(
			level[U("background")].as_string());

		web::json::array cameraPos = level[U("camera")].as_array();
		for (int i = 0;i<cameraPos.size();i++)
		{
			web::json::array enemyArr = ((cameraPos.at(i))[U("enemies")]).as_array();
			std::list<GameCharacter*> enemiesAtPos;
			for (int j = 0; j < enemies.size(); j++)
			{
				auto enemyData = (enemyArr.at(j))[U("enemy")];
				enemiesAtPos.push_back(get_enemy(enemyData));
			}
			enemies.push_back(enemiesAtPos);
		}
	}
	else
	{
		printf("Error: Can't open file: Resources/level.json\n");
		return false;
	}
	levelData.close();


	return true;
}

bool GameLevel::onEnter(GameBase *mainGame_)
{
	cameraPosCount = 0;
	mainGame = mainGame_;
	camera = { 0, 0, mainGame->getScreenW(), mainGame->getScreenH()};
	levelgrid = new SquareGrid(mainGame->getScreenW(), mainGame->getScreenH());
	createLevel();
	backGroundLevel1 = new BackGround(textureBgraund);


	players_.push_back(new Player("Resources/player.json", mainGame->getScreenW(), mainGame->getScreenH(), levelgrid, Player1));
	players.push_back(std::make_tuple(players_.front(),true));
	charactersList.push_back(players_.front());
	if (gameMode == p2Mode)
	{
		players_.push_back(new Player("Resources/player.json", mainGame->getScreenW(), mainGame->getScreenH(), levelgrid, Player2));
		charactersList.push_back(players_.back());
		players.push_back(std::make_tuple(players_.back(), true));
	}
	//charactersList.push_back(new Enemy1("Resources/enemy.json", { 380, 350 }, mainGame->getScreenW(), mainGame->getScreenH(), levelgrid, FERRIS));
	//charactersList.push_back(new Enemy1("Resources/enemy.json", { 500, 350 }, mainGame->getScreenW(), mainGame->getScreenH(), levelgrid, FERRIS));
	//charactersList.push_back(new Enemy1("Resources/enemy.json", { 560, 350 }, mainGame->getScreenW(), mainGame->getScreenH(), levelgrid, FERRIS));
	//charactersList.push_back(new Enemy1("Resources/enemy.json", { 300, 350 }, mainGame->getScreenW(), mainGame->getScreenH(), levelgrid, FERRIS));
	charactersList.splice(charactersList.end(), enemies[cameraPosCount]);

	
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