#include "BackGround.h"

BackGround::BackGround(std::string path)
	:path_(path), camera_pos(1)
{
	objTexture = NULL;
}

bool BackGround::loadMedia(SDL_Renderer* gRenderer){
	if (!LoadFromFile(path_.c_str(), gRenderer))
	{
		printf("Failed to load Backgound texture!\n");
		return false;
	}

	return true;
}

void BackGround::renderBack(SDL_Rect *camera, SDL_Renderer* gRenderer){

	if (camera->x < 0)
	{
		camera->x = 0;
	}
	if (camera->x >MAP_W - camera->w)
	{
		camera->x = MAP_W - camera->w;
	}

	render(0, 0, camera, 0, NULL, SDL_FLIP_NONE, gRenderer, camera->w, camera->h);
}

BackGround::~BackGround()
{
	free();
}