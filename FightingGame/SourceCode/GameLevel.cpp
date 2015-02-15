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

const std::string GameLevel::levelJSON = "Resources/level.json";

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

void GameLevel::ingame()
{
	std::list<GameCharacter*>::iterator it = charactersList.begin();
	while (it != charactersList.end())
	{
		if ((*it)->ready_for_delete())
		{
			if ((*it)->CharacterType() == GPLAYER)
				currentState = GAME_OVER;
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

void GameLevel::render(SDL_Renderer* renderer)
{
	backGroundLevel1->renderBack(&camera, mainGame->getRenderer());
	switch (currentState)
	{
	case INGAME:
		ingame();
		break;
	case GAME_OVER:
		Mix_HaltMusic();
		drawGameOver();
		break;
	}
}

void GameLevel::drawGameOver()
{
	int posX = mainGame->getScreenW() / 2 - gameOver->getW() / 2;
	int posY = mainGame->getScreenH() / 2 - gameOver->getH() / 2;
	gameOver->renderLabel(posX, posY, mainGame->getRenderer());
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
	for (auto enemyArr : enemies)
	{
		for (auto enemy : enemyArr)
			if (!enemy->loadMedia(mainGame->getRenderer())) return false;
	}
	gameOver->loadMedia(mainGame->getRenderer());
	charactersList.splice(charactersList.end(), enemies[cameraPosCount]);

	gMusic = Mix_LoadMUS("Resources/theme.mp3");
	if (gMusic == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}
	Mix_PlayMusic(gMusic, -1);
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
	return new Enemy1(enemyJSON, position, w, h, levelgrid, type);
}

bool GameLevel::createLevel()
{
	std::fstream levelFile;
	levelFile.open(levelJSON);
	if (levelFile.is_open())
	{
		jsonObj file = jsonObj::parse(levelFile);
		jsonObj levelData;
		playerJSON = utility::conversions::to_utf8string(
			file[U("playerJSON")].as_string());
		enemyJSON = utility::conversions::to_utf8string(
			file[U("enemyJSON")].as_string());
		if (level == Level1)
		{
			levelData = file[U("Level1")];
		}
		else if (level == Level2)
		{
			levelData = file[U("Level2")];
		}
		textureBgraund = utility::conversions::to_utf8string(
			levelData[U("background")].as_string());
		

		web::json::array cameraPos = levelData[U("camera")].as_array();
		for (int i = 0;i<cameraPos.size();i++)
		{
			web::json::array enemyArr = ((cameraPos.at(i))[U("enemies")]).as_array();
			std::list<GameCharacter*> enemiesAtPos;
			for (int j = 0; j < enemyArr.size(); j++)
			{
				auto enemyData = (enemyArr.at(j))[U("enemy")];
				enemiesAtPos.push_back(get_enemy(enemyData));
			}
			enemies.push_back(enemiesAtPos);
		}
		backGroundLevel1 = new BackGround(textureBgraund);

		createPlayer(Player1);
		if (gameMode == p2Mode) createPlayer(Player2);

		gameOver = new GameLabel(file[U("GameOverLabel")], 
			mainGame->getScreenW(), mainGame->getScreenH());
	}
	else
	{
		printf("Error: Can't open file: Resources/level.json\n");
		return false;
	}
	levelFile.close();


	return true;
}

void GameLevel::createPlayer(int id)
{
	int w = mainGame->getScreenW();
	int h = mainGame->getScreenH();
	players_.push_back(new Player(playerJSON, w, h, levelgrid, id));
	players.push_back(std::make_tuple(players_.back(), true));
	charactersList.push_back(players_.back());
}

bool GameLevel::onEnter(GameBase *mainGame_)
{
	cameraPosCount = 0;
	mainGame = mainGame_;
	currentState = INGAME;

	camera = { 0, 0, mainGame->getScreenW(), mainGame->getScreenH()};
	levelgrid = new SquareGrid(mainGame->getScreenW(), mainGame->getScreenH());
	if (!createLevel()) return false;
	
	return LoadObjects();
}

bool GameLevel::onExit()
{
	return true;
}

GameLevel::~GameLevel()
{
	Mix_FreeMusic(gMusic);
	gMusic = NULL;
	delete gameOver;
	delete backGroundLevel1;
	delete levelgrid;
}