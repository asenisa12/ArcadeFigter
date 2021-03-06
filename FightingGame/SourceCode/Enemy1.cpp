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
	prevTargetLocation = {0,0};
	//target_ = NULL;
}



void Enemy1::loadData(std::string path)
{
	std::fstream jsonFile;
	jsonFile.open(path);
	if (jsonFile.is_open())
	{
		Document allData;
		std::string content((std::istreambuf_iterator<char>(jsonFile)), std::istreambuf_iterator<char>());
		allData.Parse(content.c_str());
		Value& enemyArr = allData["enemies"];
		Value& attr = enemyArr[enemyID];

		path_ = attr["texture"].GetString();
		MAX_HEALTH = attr["MAX_HEALTH"].GetInt();
		DAMAGE = attr["DAMAGE"].GetInt();
		CLIP_H = attr["CLIP_H"].GetInt();
		CLIP_W = attr["CLIP_W"].GetInt();

		animFrameSize.push_back(attr["WALKING_FRAMES_END"].GetInt());
		animFrameSize.push_back(attr["PUNCHING_FRAMES_END"].GetInt());
		animFrameSize.push_back(attr["FALLING_FRAMES_END"].GetInt());

		SHIFTING_PERCENTIGE = allData["SHIFTING_PERCENTIGE"].GetDouble();
		DIFF_BY_X_PERCENTIGE = allData["DIFF_BY_X_PERCENTIGE"].GetDouble();
		DIFF_BY_Y_PERCENTIGE = allData["DIFF_BY_Y_PERCENTIGE"].GetDouble();

		wooshSound = loadWAV(allData["wooshSound"].GetString());
		punchSound = loadWAV(allData["punchSound"].GetString());

		health = MAX_HEALTH;
		DEATH_FRAME = 4 * FRAMES_DELIMITOR;
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
	if (prevTargetLocation != currentGoal || path.empty())
		path = (std::async(&Enemy1::getPath, this)).get();

	if (!path.empty()){
		destX = path.back().X + squareSize() / 2;
		destY = path.back().Y - squareSize() / 2 - mHigth + mHigth / 5;
		path.pop_back();
	}
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

			if (posX_ == destX && posY_ == destY && !path.empty())
			{
				changeSquare(getLocationRow(path.back()), getLocationCol(path.back()));
				Row_ = getLocationRow(currentSquare[FIRST_SQUARE_ID]);
				Col_ = getLocationCol(currentSquare[FIRST_SQUARE_ID]);
			}
		}
		else
		{	
				findDestination();
		}
	}
}

//gets the forth square next to the player
Location Enemy1::getGoalSquare()
{
	if (target_ != NULL)
	{
		if (target_->getHealth() > 0)
		{
			Location playerLocation = target_->getCurrSquare()[FIRST_SQUARE_ID];
			int playerRow = getLocationRow(playerLocation);
			int playerCol = getLocationCol(playerLocation);
			
			//if player is in right
			if (target_->getX() > posX_)
			{
				playerCol -= SQUARE_DIFF;
			}
			//if player is in left
			else
			{
				playerCol += SQUARE_DIFF;
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

bool Enemy1::synchroniseFlip(GameCharacter* player)
{
	if (characterInLeft(player) && player->getFlipType() == SDL_FLIP_NONE)
	{
		flipType = SDL_FLIP_HORIZONTAL;
		return true;
	}
	if (characterInRigh(player) && player->getFlipType() == SDL_FLIP_HORIZONTAL)
	{
		flipType = SDL_FLIP_NONE;
		return true;
	}
	return false;
}

bool Enemy1::player_punching()
{
	for (auto player : players)
	{
		auto target = std::get<TARGET>(player);
		if (target->punching())
		{
			return synchroniseFlip(target);
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
			&& (player_->getCondition() != MOVING && player_->getCondition() != JUMPING) &&
			abs(player_->getRow() - Row_) <= 2 && abs(player_->getCol() - Col_)>3)
		{
			synchroniseFlip(player_);
			if (frame / FRAMES_DELIMITOR == 3)
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
	if (frame / FRAMES_DELIMITOR >= Clips.size())
	{
		frame = firstclip;
	}
	prevTargetLocation = currentGoal;
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
		frame = DEATH_FRAME;
	}
	resizeClips(&Clips[frame / FRAMES_DELIMITOR]);
}