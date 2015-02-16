#include "Player.h"

Player::Player(std::string jsonPath, int screenW, int screenH, SquareGrid *grid, int player)
	:playerID(player), GameCharacter(grid, GPLAYER, screenW, screenH), sPunch(false), rPunch(false)
{
	frame = 0;
	int add = 0;
	camera_pos = 1;

	loadData(jsonPath);
	//movement speed == 0.7% from screen w
	movSpeed =screenW_*0.007;
	objTexture = NULL;
	setGridAttributes(grid->getLocation(Row_, Col_));
}

void Player::loadData(std::string path)
{

	std::fstream jsonFile;
	jsonFile.open(path);
	if (jsonFile.is_open())
	{

		jsonObj player = jsonObj::parse(jsonFile);
		jsonObj attr;
		if (playerID == Player1) attr = player[U("Player1")];
		else if (playerID == Player2) attr = player[U("Player2")];
	
		path_ = utility::conversions::to_utf8string(
			attr[U("texture")].as_string());

		Row_ = attr[U("row")].as_integer();
		Col_ = attr[U("col")].as_integer();

		jsonObj consts = player[U("Constants")];

		web::json::array end_frames  = consts[U("animations_size")].as_array();

		for (auto it : end_frames)
			animFrameSize.push_back(it.as_integer());

		MAX_HEALTH = consts[U("MAX_HEALTH")].as_integer();
		DAMAGE = consts[U("DAMAGE")].as_integer();
		CLIP_H = consts[U("CLIP_H")].as_integer();
		CLIP_W = consts[U("CLIP_W")].as_integer();
		health = MAX_HEALTH;

		wooshSound2 = loadWAV(utility::conversions::to_utf8string(
			attr[U("woosh2Sound")].as_string()));
		wooshSound1 = loadWAV(utility::conversions::to_utf8string(
			consts[U("woosh1Sound")].as_string()));
		punchSound = loadWAV(utility::conversions::to_utf8string(
			consts[U("punchSound")].as_string()));

	}
	else
	{
		printf("Error: Can't open file %s\n",path.c_str());
	}
	jsonFile.close();
}

Player::~Player()
{
	Mix_FreeChunk(punchSound);
	punchSound = NULL;
	Mix_FreeChunk(wooshSound1);
	wooshSound1 = NULL;
	Mix_FreeChunk(wooshSound2);
	wooshSound2 = NULL;
	free();
}

bool Player::loadMedia(SDL_Renderer* gRenderer)
{

	if (!LoadFromFile(path_.c_str(), gRenderer))
	{
		printf("Failed to load walking animation texture!\n");
		return false;
	}
	std::string animNames[] = { "WALLKING", "RUNNING", "JUMPING", 
		"PUNCH", "FALLING", "RUNPUNCH", "SUPERPUNCH", "GRAB" };
	loadAnimation(animNames);
	posToSquareMiddle();
	return true;
}


void Player::continuingAnim(bool &var, std::string name)
{
	if (animationName != name)
	{
		firstclip = 0;
		Clips = animations[name];
		frame = firstclip;
		var = true;
		animationName = name;
	}
	frame++;
	if (frame / 4 >= Clips.size()) var = false;
}

void Player::grab()
{
	currentCondition = STANDING;
	continuingAnim(grabing, "GRAB");
}

void Player::superPunch()
{
	currentCondition = PUNCHING;
	continuingAnim(sPunch, "SUPERPUNCH");
	if (frame == 1) playSound(wooshSound1);
}
void Player::runPunch()
{
	currentCondition = PUNCHING;
	continuingAnim(rPunch, "RUNPUNCH");
	if (frame == 1)	playSound(wooshSound2);
	movSpeed = 10;
}

void Player::fall()
{
	currentCondition = PUNCHED;
	animation("FALLING");
	frame++;
}

void Player::punch()
{
	currentCondition = PUNCHING;
	animation("PUNCH");
	if (frame == 1) playSound(wooshSound1);
	frame++;
}

void Player::jump()
{
	if (animationName != "JUMPING" || !jumping){
		firstclip = 0;
		Clips = animations["JUMPING"];
		frame = firstclip;
		jumping = true;
		animationName = "JUMPING";
	}

	if (jumping)
	{
		jumpH++;
		if (jumpH <= 20){
			posY_ -= 10;
		}
		else
		{
			if (jumpH == 40){
				jumping = false;
				jumpH = 0;
			}
			posY_ += 10;
		}

	}
	if (jumpH % 3 == 0)
		frame++;
}

void Player::run()
{
	animation("RUNNING");
	movSpeed = 10;

}

bool Player::checkMoveKeys()
{
	if (playerEvent.moveLeft || playerEvent.moveRight ||
		playerEvent.moveUP || playerEvent.moveDown)
	{
		return true;
	}
	return false;
	
}

void Player::manageCameraPos()
{
	Col_ = 0;
	changeSquare(Row_, Col_);
	posX_ = currentSquare[FIRST_SQUARE_ID].X + squareSize() / 2;
	shifting.X = 0;
}

bool Player::punched(std::list<GameCharacter*> characters)
{
	for (std::list<GameCharacter*>::iterator it = characters.begin(); it != characters.end(); ++it)
	{
		if ((*it)->CharacterType() == GPLAYER)
			continue;

		GameCharacter *enemy = (*it);
		otherLeft = enemy->getX();
		otherRight = enemy->getX() + enemy->getWidth() / 2;

		myBottom = posY_ + mHigth;
		myLeft = posX_ + mWidth / 5;

		if (enemy->punching() && enemy->getCondition()!=PUNCHED)
		{
			if ((characterInLeft(enemy) || characterInRigh(enemy)) &&
				abs(enemy->getRow() - Row_)<=1)
			{
				if (otherLeft > myLeft) flipType = SDL_FLIP_NONE;
				if (otherLeft < myLeft)  flipType = SDL_FLIP_HORIZONTAL;
				currentCondition = PUNCHED;
				return true;
			}
		}

		if ((characterInLeft(enemy) && flipType == SDL_FLIP_HORIZONTAL) ||
			(characterInRigh(enemy) && flipType == SDL_FLIP_NONE))
		{
			if ((animationName == "PUNCH" || animationName == "SUPERPUNCH") && 
				frame / 4 == Clips.size() / 2) playSound(punchSound);	

			if (frame / 4 == Clips.size()- 1)
			{
				playSound(punchSound);
				if (animationName == "PUNCH")
				{
					enemy->editHealth(DAMAGE);
				}
				else if (animationName == "SUPERPUNCH")
				{
					enemy->editHealth(2*DAMAGE);
				}
				else if (animationName == "RUNPUNCH")
				{
					enemy->editHealth(3*DAMAGE);
				}
			}
		}
	}
	return false;
}

void Player::push_event(std::string name)
{
	bool push = true;
	if (combos.size() > 0)
	{
		if (combos.back() == name) push = false;
	}
	if(push)combos.push_back(name);
}

void Player::handleEvent(pKeys playerKey)
{
	playerEvent = { false, false, false, false, false, false, false, false, false };
	auto currentKeyStates = SDL_GetKeyboardState(NULL);

	if (currentKeyStates[playerKey.punch])
	{
		playerEvent.normalPunch = true;
		push_event("PUNCH");
	}
	if (currentKeyStates[playerKey.grab])
	{
		playerEvent.grab = true;
		push_event("GRAB");
	}
	if (currentKeyStates[playerKey.jump] && !sPunch)
	{
		playerEvent.jump = true;
		push_event("JUMP");
	}
	if (currentKeyStates[playerKey.run])
	{
		playerEvent.run = true;
		push_event("RUN");
	}
	if (currentKeyStates[playerKey.moveD])
	{
		playerEvent.moveDown = true;
		push_event("MOVE");
	}
	if (currentKeyStates[playerKey.moveU])
	{
		playerEvent.moveUP = true;
		push_event("MOVE");
	}
	if (currentKeyStates[playerKey.moveL])
	{
		playerEvent.moveLeft = true;

		push_event("MOVE");
	}
	if (currentKeyStates[playerKey.moveR])
	{
		playerEvent.moveRight = true;
		push_event("MOVE");
	}
	if (combos.size() == 2)
	{
		if (combos.front() == "PUNCH" && !playerEvent.normalPunch)
		{
			if (combos.back() == "GRAB")
			{
				playerEvent.superPunch = true;
			}
			if (combos.back() == "RUN")
			{
				playerEvent.runPunch = true;
			}
		}
	}
	if (combos.size() >= 2)combos.pop_front();

}

bool Player::actions()
{
	if (playerEvent.superPunch || sPunch)
	{
		superPunch();
		return true;
	}
	else if (playerEvent.runPunch || rPunch)
	{
		runPunch();
		return true;
	}
	else if (playerEvent.jump || jumping)
	{
		jump();
		return true;
	}
	else if (playerEvent.grab || grabing)
	{
		grab();
		return true;
	}
	else if (playerEvent.normalPunch)
	{
		punch();
		return true;
	}
	else if (currentCondition == PUNCHED)
	{
		fall();
		return true;
	}
	return false;
}

void Player::moving()
{
	if (playerEvent.moveRight && !playerEvent.normalPunch)
	{
		moveRight();
	}
	else if (playerEvent.moveLeft && !playerEvent.normalPunch)
	{
		moveLeft();
	}
	else if (playerEvent.moveUP && !jumping)
	{
		moveUp();
	}
	else if (playerEvent.moveDown && !jumping)
	{
		moveDown();
	}
}

void Player::doActions(std::list<GameCharacter*> characters)
{
	collision(characters);
	bool ispunched = punched(characters);
	if (!actions())
	{
		if (playerEvent.run)
		{
			run();
		}
		else
		{
			animation("WALLKING");
			firstclip = 5;
		}
		//checks if the moving kes are pressed
		if (checkMoveKeys())
		{
			frame++;
		}
		else
		{
			animation("WALLKING");
			currentCondition = STANDING;
			frame = 0;
		}
	}
	moving();

	//Cycle animation
	if (frame / 4 >= Clips.size()) frame = firstclip;
	movSpeed = 3;
	manageSquareShift();
}

void Player::update(std::list<GameCharacter*> characters)
{
	if (health > 0)
	{
		doActions(characters);
	}
	else
	{
		framesToEnd--;
		animation("FALLING");
		frame = 4 * 4;
	}
	resizeClips(&Clips[frame/4]);
}


void Player::changePosX(int changedX)
{
	posX_ = changedX;
}

