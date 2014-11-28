#ifndef ENEMY_ONE
#define ENEMY_ONE

#include "GameCharacter.h"
#include <string>

class Enemy1 : public GameCharacter
{
	static const int WALKING_ANIMATION_END = 7;
	static const int PUNCHING_ANIMATION_END = 13;

	static const int CLIP_H = 100;
	static const int CLIP_W = 100;

	std::string path_;
	SDL_Rect Clips[PUNCHING_ANIMATION_END];
public:
	Enemy1(std::string path, int posX, int posY,int screenW, int screenH);
	~Enemy1();
	bool loadMedia(SDL_Renderer* renderer);
	void doActions(GameCharacter* enemy[]);
};

#endif // !ENEMY_ONE
