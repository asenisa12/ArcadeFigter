#ifndef ENEMY_ONE
#define ENEMY_ONE

#include "GameCharacter.h"
#include <string>

class Enemy1 : public GameCharacter
{
	static const int WALKING_ANIMATION_END = 7;
	static const int PUNCHING_ANIMATION_END = 12;
	static const int FALLING_ANIMATION_END = 18;

	static const double SHIFTING_PERCENTIGE;
	static const double DIFF_BY_X_PERCENTIGE;
	static const double DIFF_BY_Y_PERCENTIGE;

	static const int DAMAGE = 10;
	static const int CLIP_H = 100;
	static const int CLIP_W = 100;
	static const int FALLING_CLIP_W = 150;

	int punchStart_count;
	bool other_frame;
	bool punched;

	GameCharacter* player_;

	void moving(bool &action);
	void punch();
	void fall();
	std::string path_;
	SDL_Rect Clips[FALLING_ANIMATION_END];
public:
	Enemy1(std::string path, int posX, int posY, int screenW, int screenH, GameCharacter* player);
	~Enemy1();
	bool loadMedia(SDL_Renderer* renderer);
	void doActions(SDL_Rect* camera, std::vector<GameCharacter*> characters);
};

#endif // !ENEMY_ONE
