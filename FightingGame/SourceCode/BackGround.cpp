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

void BackGround::renderBack(SDL_Rect *clip){
	render(0, 0, clip, 0, NULL, SDL_FLIP_NONE, gRenderer_);
}

BackGround::~BackGround()
{
	free();
}