#include "BackGround.h"

BackGround::BackGround(std::string path, SDL_Renderer* gRenderer)
	:path_(path), gRenderer_(gRenderer)
{
	objTexture = NULL;
}

bool BackGround::loadMedia(){
	bool success = true;

	if (!LoadFromFile(path_.c_str(), gRenderer_))
	{
		printf("Failed to load Backgound texture!\n");
		success = false;
	}

	return success;
}

void BackGround::renderBack(SDL_Rect *camera){

	if (camera->x < 0)
	{
		camera->x = 0;
	}
	if (camera->x >3065 - camera->w)
	{
		camera->x = 3065 - camera->w;
	}

	render(0, 0, camera, 0, NULL, SDL_FLIP_NONE, gRenderer_,camera->w,camera->h);
}

BackGround::~BackGround()
{
	free();
}