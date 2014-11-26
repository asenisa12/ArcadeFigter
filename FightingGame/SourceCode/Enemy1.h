#ifndef ENEMY_ONE
#define ENEMY_ONE

#include "GameCharacter.h"
#include <string>

class Enemy1 : public GameCharacter
{
	int width, hight;
	std::string path_;
	SDL_Rect Clips[1];
public:
	Enemy1(std::string path, int posX, int posY);
	~Enemy1();
	bool loadMedia(SDL_Renderer* renderer);
};

#endif // !ENEMY_ONE
