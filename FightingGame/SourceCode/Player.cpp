#include "Player.h"

Player::Player(std::string path, SDL_Renderer* renderer,int screenW, int screenH)
	:path_(path), gRenderer(renderer), frame(0), flipType(SDL_FLIP_NONE), 
	screenH_(screenH), screenW_(screenW) 
{
	int add = 0;
	camera_pos = 1;
	movSpeed =screenW_*0.00625;
	objTexture = NULL;
}

Player::~Player()
{
	free();
}

bool Player::loadMedia()
{

	/*textureH = screenH_*(0.26); 
	textureW = screenW_*(0.17);*/
	posX = (screenW_ - textureW) / 2; 
	posY = screenH_*(0.43);

	bool success = true;

	if (!LoadFromFile(path_.c_str(), gRenderer))
	{
		printf("Failed to load walking animation texture!\n");
		success = false;
	}
	else
	{
		int x = 0;
		int y = 0;
		for (int i = 0; i < WALKING_ANIMATION_FRAMES; i++)
		{
			Clips[i].x = x;
			Clips[i].y = y;
			Clips[i].w = 75;
			Clips[i].h = 92;
			x += 75;
		}
		x = 0;
		y = 92;
		for (int i = WALKING_ANIMATION_FRAMES; i < PUNCH_ANIMATION_FRAMES; i++)
		{
			Clips[i].x = x;
			Clips[i].y = y;
			Clips[i].w = 100;
			Clips[i].h = 92;
			x += 100;
		}
	}
	return success;
}

void Player::doActions(SDL_Event e, SDL_Rect* camera)
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	if (e.type == SDL_KEYDOWN)
		frame++;
	
	if (currentKeyStates[SDL_SCANCODE_SPACE]){
		if (frame<WALKING_ANIMATION_FRAMES ||
			sframe / 4 >= PUNCH_ANIMATION_FRAMES+WALKING_ANIMATION_FRAMES)
		{
			frame = WALKING_ANIMATION_FRAMES;
		}
	}
	else
	{

		if (currentKeyStates[SDL_SCANCODE_LEFT])
		{
			if (posX > 3)
				posX -= movSpeed;
			flipType = SDL_FLIP_HORIZONTAL;
		}
		if (currentKeyStates[SDL_SCANCODE_RIGHT])
		{
			if (posX < (screenW_*(0.92)))
				posX += movSpeed;
			flipType = SDL_FLIP_NONE;
		}
		if (currentKeyStates[SDL_SCANCODE_UP])
		{
			if (posY > (screenH_*(0.42))){
				posY -= 2;
				add--;
				movSpeed -= 0.05;
			}

		}
		if (currentKeyStates[SDL_SCANCODE_DOWN])
		{
			//printf("%d\n", posY);
			if (posY < (screenH_*(0.54))){
				posY += 2;
				add++;
				movSpeed += 0.05;
			}
		}

		//Cycle animation
	}
	//printf("%d\n", add);



	if (posX > screenW_*(0.90) && camera_pos < 5){
		camera_pos++;
		camera->x += 640;
		posX = posX - screenW_*(0.90);
	}

	currentClip = &Clips[frame / 4];
	textureW = (currentClip->w * (0.0030) *screenW_) + add;
	textureH = (currentClip->h * (0.0032) *screenH_) + add;
}

void Player::renderPlayer()
{
	//Render current frame
	render(posX, posY, currentClip, 0, NULL, flipType, gRenderer, textureW, textureH);

}

void Player::changePosX(int changedX)
{
	posX = changedX;
}

int Player::getX()
{
	return posX;
}

int Player::getY()
{
	return posY;
}