#include "Player.h"

Player::Player(std::string path,int screenW, int screenH)
	:path_(path), frame(0)  	
{
	screenH_=screenH; 
	screenW_ = screenW;
	flipType = SDL_FLIP_NONE;
	int add = 0;
	camera_pos = 1;
	//movement speed == 0.625% from screen w
	movSpeed =screenW_*0.00625;
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
	for (int i = 0; i < PUNCH_ANIMATION_FRAMES_END; i++)
	{

		if (i == WALKING_ANIMATION_FRAMES_END ||
			i==JUMPING_ANIMATION_FRAMES_END || i == RUNING_ANIMATION_FRAMES_END){
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

	lastclip = PUNCH_ANIMATION_FRAMES_END;
	firstclip = JUMPING_ANIMATION_FRAMES_END * 4;
	if (frame / 4 < JUMPING_ANIMATION_FRAMES_END ||
		frame / 4 >= lastclip)
	{
		frame = firstclip;
	}
	frame++;

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

void Player::resizeClips()
{
	currentClip = &Clips[frame / 4];
	//the height or width of the texture = clip w or h * 0.3% or 0.32% 
	//+ additional x or y pos if the charracter gets closer or away
	mWidth= (currentClip->w * (0.0030) *screenW_) + add;
	mHigth = (currentClip->h * (0.0032) *screenH_) + add;
}

void Player::doActions(SDL_Event e, SDL_Rect* camera, GameCharacter* enemy[])
{
	collision(enemy);

	currentKeyStates = SDL_GetKeyboardState(NULL);
	bool punching = false;

	if (currentKeyStates[SDL_SCANCODE_SPACE] || jumping)
	{
		jump();
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
	resizeClips();
}


void Player::changePosX(int changedX)
{
	posX_ = changedX;
}

