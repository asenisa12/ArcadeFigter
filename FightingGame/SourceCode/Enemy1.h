#ifndef ENEMY_ONE
#define ENEMY_ONE

#include "GameCharacter.h"
#include "PathFinding.h"
#include <string>
#include <future>

class Enemy1 : public GameCharacter
{
	static const int WALKING_ANIMATION_END = 7;
	static const int PUNCHING_ANIMATION_END = 12;
	static const int FALLING_ANIMATION_END = 18;

	static const double SHIFTING_PERCENTIGE;
	static const double DIFF_BY_X_PERCENTIGE;
	static const double DIFF_BY_Y_PERCENTIGE;

	static const int DAMAGE = 4;
	static const int CLIP_H = 100;
	static const int CLIP_W = 100;
	static const int FALLING_CLIP_W = 150;

	int destX;
	int destY;
	int diffrenceY;
	int diffrenceX;
	int punchStart_count;
	bool other_frame;
	bool punched;
	bool action;

	GameCharacter* player_;
	Location lastPlayerSquare;
	Location currentGoal;

	void moveToPosition(int X, int Y);
	void moving();
	void punch();
	void fall();
	Location getPlayerSquare();
	std::string path_;
	std::vector<Location> getPath();
	SDL_Rect Clips[FALLING_ANIMATION_END];
public:
	Enemy1(std::string path, Location startlocation, int screenW, int screenH, 
		GameCharacter* player, SquareGrid *grid);
	~Enemy1();
	bool loadMedia(SDL_Renderer* renderer);
	void doActions(SDL_Rect* camera, std::list<GameCharacter*> characters);
};

#endif // !ENEMY_ONE
