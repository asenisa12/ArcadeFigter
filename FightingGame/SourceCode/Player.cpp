#include "Player.h"

Player::Player(std::string path,int screenW, int screenH)
	:path_(path) 	
{
	frame = 0;
	screenH_=screenH; 
	screenW_ = screenW;
	flipType = SDL_FLIP_NONE;
	int add = 0;
	camera_pos = 1;
	//movement speed == 0.625% from screen w
	movSpeed =screenW_*0.007;
	objTexture = NULL;
}

Player::~Player()
{
	free();
}

bool Player::loadMedia(SDL_Renderer* gRenderer)
{
	/*textureH = screenH_*(0.26); 
	textureW = screenW_*(0.17);*/
	posX_ = (screenW_ - mWidth) / 2; 
	posY_ = screenH_*(0.43);

	if (!LoadFromFile(path_.c_str(), gRenderer))
	{
		printf("Failed to load walking animation texture!\n");
		return false;
	}
	int x = 0;
	int y = 0;
	for (int i = 0; i < FALLING_ANIMATION_FRAMES_END; i++)
	{

		if (i == WALKING_ANIMATION_FRAMES_END ||
			i==JUMPING_ANIMATION_FRAMES_END || 
			i == RUNING_ANIMATION_FRAMES_END ||
			i== PUNCH_ANIMATION_FRAMES_END){
			x = 0;
			y += CLIP_H;
		}
		Clips[i].x = x;
		Clips[i].y = y;
		Clips[i].w = CLIP_W;
		Clips[i].h = CLIP_H;
		x += CLIP_W;
	}

	return true;
}

void Player::punch()
{
	currentCondition.punching = true;
	animation(PUNCH_ANIMATION_FRAMES_END, JUMPING_ANIMATION_FRAMES_END);
}

void Player::fall()
{
	currentCondition.punched = true;
	animation(FALLING_ANIMATION_FRAMES_END, PUNCH_ANIMATION_FRAMES_END);
}

void Player::jump()
{
	firstclip = RUNING_ANIMATION_FRAMES_END * 4;
	lastclip = JUMPING_ANIMATION_FRAMES_END;
	if (frame / 4 < RUNING_ANIMATION_FRAMES_END || !jumping)
	{
		frame = firstclip;
		jumping = true;
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
	
	firstclip = WALKING_ANIMATION_FRAMES_END * 4 + 1;
	lastclip = RUNING_ANIMATION_FRAMES_END;
	if (frame / 4 < WALKING_ANIMATION_FRAMES_END){
		frame = firstclip;
	}
	movSpeed = 10;

}

bool Player::checkKeys()
{
	if (currentKeyStates[SDL_SCANCODE_LEFT] || 
		currentKeyStates[SDL_SCANCODE_RIGHT] || 
		currentKeyStates[SDL_SCANCODE_DOWN] || 
		currentKeyStates[SDL_SCANCODE_UP])
		return true;
	return false;
	
}

void Player::manageCameraPos(SDL_Rect* camera)
{
	//last x position on screen = 90% from screen W 
	if (posX_ > screenW_*(0.90) && camera_pos < 5){
		camera_pos++;
		camera->x += screenW_;
		posX_ = posX_ - screenW_*(0.90);
	}
}

bool Player::punched(GameCharacter* enemy[], int last)
{
	for (int i = 0; i < last; i++)
	{
		enemyLeft = enemy[i]->getX();
		enemyRight = enemy[i]->getX() + enemy[i]->getWidth() / 2;

		playerBottom = posY_ + mHigth;
		playerLeft = posX_ + mWidth / 5;

		if (enemy[i]->punching() && !enemy[i]->getCondition().punched)
		{
			/*if (characterInLeft()){
				flipType = SDL_FLIP_HORIZONTAL;
			}
			else if (characterInRigh()){
				flipType = SDL_FLIP_NONE;
			}
				*/
			return true;
		}
	}
	return false;
}

void Player::doActions(SDL_Event e, SDL_Rect* camera, GameCharacter* enemy[])
{
	currentKeyStates = SDL_GetKeyboardState(NULL);
	bool punching = false;
	collision(enemy,2);
	currentCondition = { false, false, false, false };

	//printf("%d\n\n", punched);
	if (currentKeyStates[SDL_SCANCODE_SPACE] || jumping)
	{
		jump();
	}
	else if (punched(enemy,2))
	{
		fall();
	}
	else if (currentKeyStates[SDL_SCANCODE_Q])
	{
		punch();
	}
	else 
	{
		if (currentKeyStates[SDL_SCANCODE_LSHIFT])
		{
			run();
		}
		else
		{
			lastclip = WALKING_ANIMATION_FRAMES_END;
			firstclip = 5;
		}
		//checks if the moving kes are pressed
		if (checkKeys())
		{
			frame++;
		}
		else
		{
			frame = 0;
		}
	}
			
	if (currentKeyStates[SDL_SCANCODE_RIGHT] && !punching)
	{
		moveRight();
	}
	else if (currentKeyStates[SDL_SCANCODE_LEFT] && !punching)
	{
		moveLeft();
	}
	else if (currentKeyStates[SDL_SCANCODE_UP] && !jumping)
	{
		moveUp();
	}
	else if (currentKeyStates[SDL_SCANCODE_DOWN] && !jumping)
	{
		moveDown();
	}
	
	//Cycle animation
	if (frame/4 >= lastclip )
	{
		frame = firstclip;
	}
	movSpeed = 3;

	manageCameraPos(camera);
	resizeClips(Clips);
}


void Player::changePosX(int changedX)
{
	posX_ = changedX;
}

