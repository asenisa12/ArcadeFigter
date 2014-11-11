#include "Player.h"

Player::Player(std::string path, SDL_Renderer* renderer,int screenW, int screenH)
	:path_(path), gRenderer(renderer), frame(0), flipType(SDL_FLIP_NONE), 
	screenH_(screenH), screenW_(screenW) 
{
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

	textureH = screenH_*(0.26); 
	textureW = screenW_*(0.17);
	posX = (screenW_ - textureW) / 2; 
	posY = screenH_*(0.43);
	printf("%d", posY);
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
		for (int i = 0; i < 7; i++)
		{
			walkingClips[i].x = x;
			walkingClips[i].y = y;
			walkingClips[i].w = 75;
			walkingClips[i].h = 92;
			x += 75;
		}
	}
	return success;
}

void Player::doActions(SDL_Event e, SDL_Rect* camera)
{

	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

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
			textureH--;
			textureW--;
			movSpeed -= 0.05;
		}

	}
	if (currentKeyStates[SDL_SCANCODE_DOWN])
	{
		printf("%d\n", posY);
		if (posY < (screenH_*(0.54))){
			posY += 2;
			textureH++;
			textureW++;
			movSpeed += 0.05;
		}
	}

	if (e.type == SDL_KEYDOWN)
		frame++;



	//Cycle animation
	if (frame / 4 >= WALKING_ANIMATION_FRAMES)
	{
		frame = 0;
	}


	if (posX > screenW_*(0.90) && camera_pos < 5){
		camera_pos++;
		camera->x += 640;
		posX = posX - screenW_*(0.90);
	}

}

void Player::renderPlayer()
{
	//Render current frame
	SDL_Rect* currentClip = &walkingClips[frame / 4];
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