#ifndef ENEMY_ONE
#define ENEMY_ONE

#include "GameObject.h"
#include <string>

class Enemy1 : public GameObject
{
	int width, hight;
	std::string path_;
	SDL_Rect Clips[1];
public:
	Enemy1(std::string path, int posX, int posY);
	~Enemy1();
	bool loadMedia(SDL_Renderer* renderer);
	void renderEnemy1(SDL_Renderer* renderer);
};

#endif // !ENEMY_ONE
