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
	runSpeed = movSpeed * 3;
	objTexture = NULL;
	setGridAttributes(grid->getLocation(Row_, Col_));
}

void Player::loadData(std::string path)
{

	std::fstream jsonFile;
	jsonFile.open(path);
	if (jsonFile.is_open())
	{

		Document player;
		std::string content((std::istreambuf_iterator<char>(jsonFile)), std::istreambuf_iterator<char>());
		player.Parse(content.c_str());
		Value attr;
		if (playerID == Player1) attr = player["Player1"];
		else if (playerID == Player2) attr = player["Player2"];
	
		path_ = attr["texture"].GetString();

		Row_ = attr["row"].GetInt();
		Col_ = attr["col"].GetInt();

		Value& consts = player["Constants"];

		Value& end_frames  = consts["animations_size"];

		for (int i = 0; i < end_frames.Size(); i++)
			animFrameSize.push_back(end_frames[i].GetInt());

		MAX_HEALTH = consts["MAX_HEALTH"].GetInt();
		DAMAGE = consts["DAMAGE"].GetInt();
		CLIP_H = consts["CLIP_H"].GetInt();
		CLIP_W = consts["CLIP_W"].GetInt();
		health = MAX_HEALTH;

		wooshSound2 = loadWAV(attr["woosh2Sound"].GetString());
		wooshSound1 = loadWAV(consts["woosh1Sound"].GetString());
		punchSound = loadWAV(consts["punchSound"].GetString());

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
	if (frame / FRAMES_DELIMITOR >= Clips.size()) var = false;
}

void Player::grab()
{
	currentCondition = STANDING;
	continuingAnim(grabing, "GRAB");
	if (itm != NULL && !itm->grabed)
	{
		if ((posX_ + mWidth / 2 > itm->getX()) && (posX_  + mWidth / 2 < itm->getX() + itm->getW()) &&
			(getBottomY() > itm->getY()) && (getBottomY() < itm->getY() + itm->getH()*1.5))
		{
			itm->grabed = true;
			health += itm->getHealth();
			if (health > MAX_HEALTH)health = MAX_HEALTH;
		}
	}
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
	movSpeed = runSpeed;
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
		if (jumpH <= Max_jumpH/2){
			posY_ -= runSpeed;
		}
		else
		{
			if (jumpH == Max_jumpH){
				jumping = false;
				jumpH = 0;
			}
			posY_ += runSpeed;
		}

	}
	if (jumpH % 3 == 0)
		frame++;
	currentCondition = JUMPING;
}

void Player::run()
{
	animation("RUNNING");
	movSpeed = runSpeed;

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

		if (enemy->punching())
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
				frame / FRAMES_DELIMITOR == Clips.size() / 2) playSound(punchSound);

			if (frame / FRAMES_DELIMITOR  == Clips.size() - 1)
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
	if (combos.size() == ComboMoves)
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
	if (combos.size() >= ComboMoves)combos.pop_front();

}

bool Player::actions()
{
	if (playerEvent.superPunch || sPunch)
	{
		superPunch();
		return true;
	}
	if (playerEvent.runPunch || rPunch)
	{
		runPunch();
		return true;
	}
	if (playerEvent.jump || jumping)
	{
		jump();
		return true;
	}
	if (playerEvent.grab || grabing)
	{
		grab();
		return true;
	}
	if (playerEvent.normalPunch)
	{
		punch();
		return true;
	}
	if (currentCondition == PUNCHED)
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
	currentCondition = STANDING;
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
			firstclip = StandingFRAME;
		}
		//checks if the moving kes are pressed
		if (checkMoveKeys())
		{
			frame++;
		}
		else
		{
			animation("WALLKING");
			frame = 0;
		}
	}
	moving();

	//Cycle animation
	if (frame / FRAMES_DELIMITOR >= Clips.size()) frame = firstclip;

	//calculating the movement speed by geting 0,7% from the screen W
	movSpeed = screenW_*0.007;
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
		frame = DyingFRAME *FRAMES_DELIMITOR;
	}
	resizeClips(&Clips[frame/FRAMES_DELIMITOR]);
}


void Player::changePosX(int changedX)
{
	posX_ = changedX;
}

