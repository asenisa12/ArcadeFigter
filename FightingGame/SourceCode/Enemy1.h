#ifndef ENEMY_ONE
#define ENEMY_ONE

#include "GameCharacter.h"
#include <string>

class Enemy1 : public GameCharacter
{
	std::string path_;
	SDL_Rect Clips[1];
public:
	Enemy1(std::string path, int posX, int posY,int screenW, int screenH);
	~Enemy1();
	bool loadMedia(SDL_Renderer* renderer);
	void doActions(GameCharacter* enemy[]);
};

#endif // !ENEMY_ONE
