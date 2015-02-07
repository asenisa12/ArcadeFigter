#ifndef GAME_BUTTON
#define GAME_BUTTON

#include "GameObject.h"
#include <SDL_Image.h>

class GameButton :public GameObject
{
	static enum{ MOUSE_OUT = 0, MOUSE_OVER = 1, MOUSE_CLICK = 2 };
	static const int BUTTON_CLIPS = 3;
	int CLIP_W;
	int CLIP_H;

	std::string path_;
	int framesToEnd;
	int currentState;
	int width, height;
	SDL_Rect buttonState[BUTTON_CLIPS];
public:
	GameButton(int screenW, int screenH, jsonObj buttonData);
	bool loadMedia(SDL_Renderer* gRenderer);
	bool isPressed(SDL_Event *keyEvent);
	void renderButton(SDL_Renderer *gRender);
};

#endif