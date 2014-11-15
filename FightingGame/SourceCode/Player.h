#ifndef PLAYER
#define PLAYER

#include "GameObject.h"

class Player : public GameObject
{	
	SDL_RendererFlip flipType;
	int add;
	int camera_pos;
	int posX, posY;
	double textureW, textureH;
	int frame;
	int screenH_, screenW_;
	double movSpeed;
	bool jumping;
	int jumpH;
	static const int WALKING_ANIMATION_FRAMES_END = 12;
	static const int RUNING_ANIMATION_FRAMES_END = 21;
	static const int JUMPING_ANIMATION_FRAMES_END = 25;
	static const int PUNCH_ANIMATION_FRAMES_END = 36;

	SDL_Rect* currentClip;
	SDL_Rect Clips[PUNCH_ANIMATION_FRAMES_END];
	SDL_Renderer* gRenderer;
	std::string path_;
public:
	Player(std::string path, SDL_Renderer* renderer, int screenH, int screenW);
	~Player();
	bool loadMedia();
	void doActions(SDL_Event e, SDL_Rect* camera);
	void renderPlayer();
	void changePosX(int changedX);
	int getX();
	int getY();
};

#endif