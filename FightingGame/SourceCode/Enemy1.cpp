#include "Enemy1.h"

Enemy1::Enemy1(std::string path, Location startlocation, int screenW, int screenH,
	GameCharacter* player, SquareGrid *grid, int id)
	:player_(player), action(false), enemyID(id), GameCharacter(grid, GENEMY, screenW, screenH)
{
	loadData(path);
	punchStart_count = 0;
	//movement speed == 0.5% from screen width
	movSpeed = screenW_*0.005;	
	setGridAttributes(startlocation);
	PrevSquare = currentSquare[FIRST_SQUARE_ID];
	Row_ = getLocationRow(startlocation);
	Col_ = getLocationCol(startlocation);
}



void Enemy1::loadData(std::string path)
{
	std::fstream jsonFile;
	jsonFile.open(path);
	if (jsonFile.is_open())
	{
		jsonObj enemy = jsonObj::parse(jsonFile);
		jsonObj attr;

		switch (enemyID)
		{
		case FERRIS:
			attr = enemy[U("Ferrys")];
			break;
		}
		path_ = utility::conversions::to_utf8string(attr[U("texture")].as_string());
		MAX_HEALTH = attr[U("MAX_HEALTH")].as_integer();
		DAMAGE = attr[U("DAMAGE")].as_integer();
		CLIP_H = attr[U("CLIP_H")].as_integer();
		CLIP_W = attr[U("CLIP_W")].as_integer();
		FALLING_CLIP_W = attr[U("FALLING_CLIP_W")].as_integer();
		jsonObj variables = enemy[U("variables")];
		WALKING_ANIMATION_END = variables[U("WALKING_ANIMATION_END")].as_integer();
		PUNCHING_ANIMATION_END = variables[U("PUNCHING_ANIMATION_END")].as_integer();
		FALLING_ANIMATION_END = variables[U("FALLING_ANIMATION_END")].as_integer();
		SHIFTING_PERCENTIGE = variables[U("SHIFTING_PERCENTIGE")].as_double();
		DIFF_BY_X_PERCENTIGE = variables[U("DIFF_BY_X_PERCENTIGE")].as_double();
		DIFF_BY_Y_PERCENTIGE = variables[U("DIFF_BY_Y_PERCENTIGE")].as_double();
		Clips = new SDL_Rect[FALLING_ANIMATION_END];
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

	int x = 0;
	int y = 0;
	for (int i = 0; i < FALLING_ANIMATION_END;i++)
	{
		if (i == WALKING_ANIMATION_END || i== PUNCHING_ANIMATION_END)
		{
			x = 0;
			y += CLIP_H;
		}
		Clips[i] = { x, y, CLIP_W, CLIP_H };
		if (i == 15 || i == 16)
		{
			x += FALLING_CLIP_W;
		}
		else x += CLIP_W;
		
	}

	currentClip = &Clips[0];
	frame = 0;

	resizeClips(Clips);
	posToSquareMiddle();
	destY = posY_;
	destX = posX_;

	return true;


}

Enemy1::~Enemy1()
{
	delete[] Clips;
	free();
}

void Enemy1::punch()
{
	currentCondition = PUNCHING;
	animation(PUNCHING_ANIMATION_END, WALKING_ANIMATION_END);
}

void Enemy1::fall()
{
	currentCondition = PUNCHED;
	animation(FALLING_ANIMATION_END, PUNCHING_ANIMATION_END);
}

void Enemy1::findDestination()
{
	path = (std::async(&Enemy1::getPath, this)).get();

	for (;;){
		if (currentSquare[FIRST_SQUARE_ID] == path.back() && path.back() != currentGoal)
		{
			path.pop_back();
		}
		else break;
	}
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
				printf("DESTINATION posX: %d posY: %d\n", destX, destY);
		}
	}
}

Location Enemy1::getPlayerSquare()
{
	Location playerLocation = player_->getCurrSquare()[FIRST_SQUARE_ID];
	int playerRow = getLocationRow(playerLocation);
	int playerCol = getLocationCol(playerLocation);
	playerCol += 4;
	return levelGrid->getLocation(playerRow-1, playerCol);
}

std::vector<Location> Enemy1::getPath()
{
	std::unordered_map<Location, Location, LocationHash, Equal> came_from;
	std::unordered_map<Location, int, LocationHash, Equal> cost_so_far;
	path_search(*levelGrid, currentSquare[FIRST_SQUARE_ID], currentGoal, came_from, cost_so_far);
	auto path = reconstruct_path(currentSquare[FIRST_SQUARE_ID], currentGoal, came_from);
	return path;
}

void Enemy1::doActions(SDL_Rect* camera, std::list<GameCharacter*> characters)
{
		printf("aa%d\n", MAX_HEALTH);
	punched = false;
	collision(characters);
	otherLeft = player_->getX();
	otherRight = player_->getX() + player_->getWidth() / 2;
	currentCondition = STANDING;
	action = false;
	currentGoal = getPlayerSquare();

	if (player_->punching() &&  (characterInLeft(player_) || characterInRigh(player_)))
	{
		fall();
	}
	else
	{
		lastclip = WALKING_ANIMATION_END;
		firstclip = 0;
		
		moving();

		if (action)
		{
			frame++;
		}
		else if ((characterInLeft(player_) || characterInRigh(player_)) && !punched
			&& player_->getCondition()!=MOVING)
		{
			if (frame / 4 == PUNCHING_ANIMATION_END - 1)
				player_->editHealth(DAMAGE);
			punch();
		}

	}
	if (frame/4 >= lastclip)
	{
		frame = firstclip;
	}
	resizeClips(Clips);
}