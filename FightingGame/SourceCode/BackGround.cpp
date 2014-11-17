#include "BackGround.h"

BackGround::BackGround(std::string path, SDL_Renderer* gRenderer)
	:path_(path), gRenderer_(gRenderer), camera_pos(1)
{
	objTexture = NULL;
}

bool BackGround::loadMedia(){
	if (!LoadFromFile(path_.c_str(), gRenderer_))
	{
		printf("Failed to load Backgound texture!\n");
		return false;
	}

	return true;
}

void BackGround::renderBack(SDL_Rect *camera, int playerX){

	

	if (camera->x < 0)
	{
		camera->x = 0;
	}
	if (camera->x >MAP_W - camera->w)
	{
		camera->x = MAP_W - camera->w;
	}

	render(0, 0, camera, 0, NULL, SDL_FLIP_NONE, gRenderer_, camera->w, camera->h);
}

BackGround::~BackGround()
{
	free();
}