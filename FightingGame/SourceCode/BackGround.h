#ifndef BACKGROUND
#define BACKGOUND

#include "GameObject.h"
#include <string>
#include <SDL.h>

class BackGround :public GameObject
{
	static const int MAP_W = 3065;

	int camera_pos;
	std::string path_;
	SDL_Renderer* gRenderer_;
public:
	BackGround(std::string path, SDL_Renderer* gRenderer);
	void renderBack(SDL_Rect *clip,int playerX);
	bool loadMedia();
	~BackGround();
};



#endif