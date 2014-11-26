#ifndef PLAYER
#define PLAYER

#include "GameCharacter.h"
#include <cmath>  

class Player : public GameCharacter
{	
	struct direction{ bool right; bool left; bool up; bool down; };
	struct direction moveDir;
	int add;
	int camera_pos;
	int jumpH;
	int firstclip;
	int lastclip;
	int frame;
	int screenH_, screenW_;
	double movSpeed;
	bool jumping;
	const Uint8* currentKeyStates;

	static const int WALKING_ANIMATION_FRAMES_END = 12;
	static const int RUNING_ANIMATION_FRAMES_END = 21;
	static const int JUMPING_ANIMATION_FRAMES_END = 25;
	static const int PUNCH_ANIMATION_FRAMES_END = 36;

	static const int CLIP_H = 100;
	static const int CLIP_W = 100;

	SDL_Rect Clips[PUNCH_ANIMATION_FRAMES_END];
	
	std::string path_;

	void punch();
	void jump();
	void run();
	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();
	bool checkKeys();

	void resizeClips();
	void collision(GameCharacter* enemy[]);
	void manageCameraPos(SDL_Rect* camera);

public:
	Player(std::string path, int screenH, int screenW);
	~Player();
	bool loadMedia(SDL_Renderer* gRenderer);
	//implements key presses
	void doActions(SDL_Event e, SDL_Rect* camera, GameCharacter* enemy[]);
	void changePosX(int changedX);
};

#endif