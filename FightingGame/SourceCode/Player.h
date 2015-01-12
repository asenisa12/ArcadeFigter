#ifndef PLAYER
#define PLAYER

#include "GameCharacter.h"
#include <cmath>  

class Player : public GameCharacter
{	
	int add;
	int camera_pos;
	int jumpH;

	bool jumping;
	const Uint8* currentKeyStates;

	static const int WALKING_ANIMATION_FRAMES_END = 12;
	static const int RUNING_ANIMATION_FRAMES_END = 21;
	static const int JUMPING_ANIMATION_FRAMES_END = 25;
	static const int PUNCH_ANIMATION_FRAMES_END = 36;
	static const int FALLING_ANIMATION_FRAMES_END = 45;

	static const int MAX_HEALTH = 200;
	static const int DAMAGE = 50;
	static const int CLIP_H = 100;
	static const int CLIP_W = 100;

	SDL_Rect Clips[FALLING_ANIMATION_FRAMES_END];
	
	std::string path_;

	bool punched(std::list<GameCharacter*> characters);
	void fall();
	void punch();
	void jump();
	void run();
	void checkCondition();
	bool checkMoveKeys();

	void manageCameraPos(SDL_Rect* camera);

public:
	Player(std::string path, int screenW, int screenH, Location startingLocation, SquareGrid *grid);
	~Player();
	bool loadMedia(SDL_Renderer* gRenderer);
	//implements key presses
	void doActions(SDL_Rect* camera, std::list<GameCharacter*> characters);
	void changePosX(int changedX);
};

#endif