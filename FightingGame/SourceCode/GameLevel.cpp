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

	switch (mainGame->gameEvent.type)
	{
	case SDL_KEYUP:
		if (mainGame->gameEvent.key.keysym.sym == SDLK_ESCAPE){
			quitLevel = true;
		}
	}
	
	manage_camera();
	if (gameMode == p2Mode)
		players_.back()->handleEvent(player2Keys);
}

void GameLevel::update(GameStateMachine *stateMachine)
{
	switch (currentState)
	{
	case INGAME:
		enemy_add_target();
		charactersList.sort([](GameCharacter* struct1, GameCharacter* struct2)
			{return (struct1->getBottomY()< struct2->getBottomY()); });
		for (auto character : charactersList)
			character->update(charactersList);
		break;
	}
	if (quitLevel)
	{
		Mix_HaltMusic();
		stateMachine->popState();
		players.clear();
	}
}

void GameLevel::ingame()
{
	std::list<GameCharacter*>::iterator it = charactersList.begin();
	Item *itm = (*items)[currentState];
	itm->render(mainGame->getRenderer());

	while (it != charactersList.end())
	{
		if ((*it)->ready_for_delete())
		{
			if ((*it)->CharacterType() == GPLAYER)
				currentState = GAME_OVER;
			if ((*it)->CharacterType() == GENEMY)
				livingEnemies--;
			it = charactersList.erase(it);
		}
		else
		{
			(*it)->renderCharacter(mainGame->getRenderer());
			++it;
		}
	}
	drawPlayerHealthBar();
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
		drawLabel(gameOver);
		break;
	case WIN:
		Mix_HaltMusic();
		drawLabel(youWin);
		break;
	}
}

void GameLevel::drawLabel(GameLabel *label)
{
	int posX = mainGame->getScreenW() / 2 - label->getW() / 2;
	int posY = mainGame->getScreenH() / 2 - label->getH() / 2;
	label->renderLabel(posX, posY, mainGame->getRenderer());
}

void GameLevel::drawPlayerHealthBar()
{
	int y = 0;
	std::list<Player*>::iterator it = players_.begin();
	for (auto hBar : healthBars)
	{
		hBar->renderBar(mainGame->getRenderer(), (*it)->getHealth(), y);
		y += hBar->getH();
		if (gameMode == p2Mode) it++;
	}
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
	youWin->loadMedia(mainGame->getRenderer());

	charactersList.insert(charactersList.end(), 
		enemies[cameraPosCount].begin(), enemies[cameraPosCount].end());

	gMusic = Mix_LoadMUS(levelTheme.c_str());
	if (gMusic == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}
	Mix_PlayMusic(gMusic, -1);
	for (auto hBar : healthBars)
	{
		hBar->loadMedia(mainGame->getRenderer());
		hBar->setMaxHealth(players_.back()->getHealth());
	}
	items->loadMedia(mainGame->getRenderer());
}

void GameLevel::manage_camera()
{
	if (cameraPosCount == CAMERA_POSITIONS-1 && livingEnemies <= 0)
	{
		currentState = WIN;
	}
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
		if (playersAtEndOfCAmera == players.size() && livingEnemies<=0){
			for (auto player : players_) player->manageCameraPos();
			camera.x +=backGroundLevel1->getWidth()/5;
			cameraPosCount++;
			livingEnemies = enemies[cameraPosCount].size();
			charactersList.insert(charactersList.end(), 
				enemies[cameraPosCount].begin(), enemies[cameraPosCount].end());
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

void GameLevel::enemy_add_target()
{
	for(int i = 0; i<players.size(); i++)
	{
		if (std::get<AVAILABLE>(players[i]))
		{
			enemies[cameraPosCount].sort([](GameCharacter* struct1, GameCharacter* struct2)
			{return (struct1->getX()< struct2->getX()); });

			for (auto enemy : enemies[cameraPosCount])
			{
				if (enemy->getTarget() == NULL && enemy->getHealth()>0)
				{
					enemy->setTarget(std::get<TARGET>(players[i]), i);
					std::get<AVAILABLE>(players[i]) = false;
					break;
				}
			}
		}
	}
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
		levelTheme = utility::conversions::to_utf8string(
			levelData[U("levelTheme")].as_string());

		int w = mainGame->getScreenW();
		int h = mainGame->getScreenH();
		
		int healthsCount = 1;
		if (gameMode == p2Mode) healthsCount++;

		web::json::array hBars = file[U("healthBar")].as_array();
		for (int i = 0; i < healthsCount; i++)
			healthBars.push_back(new HealthBar(hBars.at(i), w, h));

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
		youWin = new GameLabel(file[U("YouWinLabel")],
			mainGame->getScreenW(), mainGame->getScreenH());
		items = new Items(file[U("Items")], levelgrid, CAMERA_POSITIONS);
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
	quitLevel = false;
	mainGame = mainGame_;
	currentState = INGAME;

	camera = { 0, 0, mainGame->getScreenW(), mainGame->getScreenH()};
	levelgrid = new SquareGrid(mainGame->getScreenW(), mainGame->getScreenH());

	if (!createLevel()) return false;

	livingEnemies = enemies[cameraPosCount].size();
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
	delete youWin;
	delete backGroundLevel1;
	delete levelgrid;
	delete items;
}