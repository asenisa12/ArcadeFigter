#include "Enemy1.h"

Enemy1::Enemy1(std::string path, Location startlocation, int screenW, int screenH, SquareGrid *grid, int id)
	:action(false), enemyID(id), GameCharacter(grid, GENEMY, screenW, screenH), playersCOUNT(players.size())
{
	flipType = SDL_FLIP_HORIZONTAL;
	loadData(path);
	punchStart_count = 0;
	//movement speed == 0.5% from screen width
	movSpeed = screenW_*0.005;	
	setGridAttributes(startlocation);
	PrevSquare = currentSquare[FIRST_SQUARE_ID];
	Row_ = getLocationRow(startlocation);
	Col_ = getLocationCol(startlocation);
	//target_ = NULL;
}



void Enemy1::loadData(std::string path)
{
	std::fstream jsonFile;
	jsonFile.open(path);
	if (jsonFile.is_open())
	{
		jsonObj allData = jsonObj::parse(jsonFile);
		web::json::array enemyArr = allData[U("enemies")].as_array();
		jsonObj attr = enemyArr.at(enemyID);

		path_ = utility::conversions::to_utf8string(attr[U("texture")].as_string());
		MAX_HEALTH = attr[U("MAX_HEALTH")].as_integer();
		DAMAGE = attr[U("DAMAGE")].as_integer();
		CLIP_H = attr[U("CLIP_H")].as_integer();
		CLIP_W = attr[U("CLIP_W")].as_integer();

		animFrameSize.push_back(attr[U("WALKING_FRAMES_END")].as_integer());
		animFrameSize.push_back(attr[U("PUNCHING_FRAMES_END")].as_integer());
		animFrameSize.push_back(attr[U("FALLING_FRAMES_END")].as_integer());

		SHIFTING_PERCENTIGE = allData[U("SHIFTING_PERCENTIGE")].as_double();
		DIFF_BY_X_PERCENTIGE = allData[U("DIFF_BY_X_PERCENTIGE")].as_double();
		DIFF_BY_Y_PERCENTIGE = allData[U("DIFF_BY_Y_PERCENTIGE")].as_double();

		wooshSound = loadWAV(utility::conversions::to_utf8string(
			allData[U("wooshSound")].as_string()));
		punchSound = loadWAV(utility::conversions::to_utf8string(
			allData[U("punchSound")].as_string()));

		health = MAX_HEALTH;
	}
	else
	{
		printf("Error: Can't open file %s\n", path.c_str());
	}
	jsonFile.close();
}

bool Enemy1::loadMedia(SDL_Renderer* gRenderer)
{
	if (!LoadFromFile(path_.c_str(), gRenderer))
	{
		printf("Failed to load walking animation texture!\n");
		return false;
	}
	std::string animNames[] = { "WALLKING", "PUNCH", "FALLING" };
	loadAnimation(animNames);
	destY = posY_;
	destX = posX_;

	return true;


}

Enemy1::~Enemy1()
{
	Mix_FreeChunk(punchSound);
	punchSound = NULL;
	Mix_FreeChunk(wooshSound);
	wooshSound = NULL;
	free();
}

void Enemy1::punch()
{
	currentCondition = PUNCHING;
	animation("PUNCH");
	frame++;
	if (frame == 1) playSound(wooshSound);
}

void Enemy1::fall()
{
	currentCondition = PUNCHED;
	animation("FALLING");
	frame++;
}

void Enemy1::findDestination()
{
	path = (std::async(&Enemy1::getPath, this)).get();
	if (abs(target_->getCol() - Col_) > 2)
	{
		for (;;){
			if (currentSquare[FIRST_SQUARE_ID] == path.back() && path.back() != currentGoal)
			{
				path.pop_back();
			}
			else break;
		}
		if (target_->getX() > posX_ && currentSquare[FIRST_SQUARE_ID].X == path.back().X && path.size()>1)
			path.pop_back();

	}
	else {
		for (int i = 0; i < path.size() / 2; i++)
			path.pop_back();
	}

	/*for (auto it : path)
		printf("X:%dY:%d\n", it.X, it.Y);*/
	destX = path.back().X + squareSize() / 2;
	destY = path.back().Y - squareSize() / 2 - mHigth + mHigth/5;
}

void Enemy1::moveToPosition(int X, int Y)
{
	diffrenceY = abs(Y+mHigth - getBottomY());
	diffrenceX = abs(posX_ - X);
	if (diffrenceY >screenH_ * DIFF_BY_Y_PERCENTIGE)
	{
		if (Y > posY_)
		{
			if (moveDir.down)
			{
				moveDown();
				action = true;
				return;
			}

		}
		else if (Y <posY_)
		{
			if (moveDir.up)
			{
				moveUp();
				action = true;
				return;
			}
		}
	}
	else if (diffrenceX >screenW_ * DIFF_BY_X_PERCENTIGE)
	{
		if (posX_ > X)
		{
			if (moveDir.left)
			{
				moveLeft();
				action = true;
				return;
			}
		}
		else if (posX_ < X)
		{
			if (moveDir.right)
			{
				moveRight();
				action = true;
				return;
			}
		}
	}
	if (diffrenceY < screenH_ * DIFF_BY_Y_PERCENTIGE+5)
	{
		posX_ =X;
		action = true;
	}
	if (diffrenceX < screenW_ * DIFF_BY_X_PERCENTIGE)
	{
		posY_ = Y;
		action = true;
	}
}

void Enemy1::moving()
{
	if (currentGoal != currentSquare[FIRST_SQUARE_ID])
	{

		if (posX_ != destX || posY_ != destY)
		{
			moveToPosition(destX, destY);

			if (posX_ == destX && posY_ == destY)
			{
				changeSquare(getLocationRow(path.back()), getLocationCol(path.back()));
				Row_ = getLocationRow(currentSquare[FIRST_SQUARE_ID]);
				Col_ = getLocationCol(currentSquare[FIRST_SQUARE_ID]);
			}
		}
		else
		{	
				findDestination();
				printf("posX: %d posY: %d\n", posX_, posY_);
				printf("DESTINATION posX: %d posY: %d\n", destX, destY);
		}
	}
}

Location Enemy1::getGoalSquare()
{

	//getTarget();
	if (target_ != NULL)
	{
		if (target_->getHealth() > 0)
		{
			Location playerLocation = target_->getCurrSquare()[FIRST_SQUARE_ID];
			int playerRow = getLocationRow(playerLocation);
			int playerCol = getLocationCol(playerLocation);
			
			if (target_->getX() > posX_)
			{
				playerCol -= 4;
			}
			else
			{
				playerCol += 4;
			}
			return levelGrid->getLocation(playerRow-1, playerCol);
		}
		else
		{
			target_ = NULL; 
			return currentSquare[0];
		}
	}
	else 
	{
		if (target_ == NULL) return currentSquare[0];
	}

}

std::vector<Location> Enemy1::getPath()
{
	std::unordered_map<Location, Location, LocationHash, Equal> came_from;
	std::unordered_map<Location, int, LocationHash, Equal> cost_so_far;
	path_search(*levelGrid, currentSquare[FIRST_SQUARE_ID], currentGoal, came_from, cost_so_far);
	auto path = reconstruct_path(currentSquare[FIRST_SQUARE_ID], currentGoal, came_from);
	return path;
}

bool Enemy1::player_punching()
{
	for (auto player : players)
	{
		auto target = std::get<TARGET>(player);
		if (target->punching())
		{
			if (characterInLeft(target) && target->getFlipType() == SDL_FLIP_NONE)
			{
				flipType = SDL_FLIP_HORIZONTAL;
				return true;
			}
			if (characterInRigh(target) && target->getFlipType() == SDL_FLIP_HORIZONTAL)
			{
				flipType = SDL_FLIP_NONE;
				return true;
			}
		}

	}
	return false;
}



void Enemy1::punch_players()
{
	bool hitPlayer = false;
	for (auto player : players)
	{
		GameCharacter *player_ = std::get<TARGET>(player);
		if ((characterInLeft(player_) || characterInRigh(player_)) && !player_punching()
			&& player_->getCondition() != MOVING && abs(player_->getRow()-Row_)<=1)
		{
			if (frame / 4 == 3)
			{
				player_->editHealth(DAMAGE);
				playSound(punchSound);
			}
			hitPlayer = true;
		}
	}
	if (hitPlayer) punch();
}


void Enemy1::doActions(std::list<GameCharacter*> characters)
{
	punched = false;
	moveDir = { true, true, true, true };
	//collision(characters);
	currentCondition = STANDING;
	action = false;
	currentGoal = getGoalSquare();

	if (player_punching())
	{
		fall();
	}
	else
	{
		animation("WALLKING");

		if (currentGoal != currentSquare[0])
		{
			moving();
		}

		if (action)
		{
			frame++;
		}
		else punch_players();

	}
	if (frame / 4 >= Clips.size())
	{
		frame = firstclip;
	}
}
void Enemy1::update(std::list<GameCharacter*> characters)
{
	if (health > 0)
	{
		doActions(characters);
	}
	else
	{
		if (target_ != NULL)
		{
			std::get<AVAILABLE>(players[playernum]) = true;
			target_ = NULL;
		}
		framesToEnd--;
		animation("FALLING");
		frame =4* 4;
	}
	resizeClips(&Clips[frame/4]);
}