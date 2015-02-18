#include "GameButton.h"

GameButton::GameButton(int screenW, int screenH, jsonObj buttonData)
	:framesToEnd(0), buttonPressed(false)
{
	width = screenW / buttonData[U("widthDivide")].as_integer();
	height = screenH / buttonData[U("heightDivide")].as_integer();
	CLIP_W = buttonData[U("CLIP_W")].as_integer();
	CLIP_H = buttonData[U("CLIP_H")].as_integer();
	path_ = utility::conversions::to_utf8string(buttonData[U("texture")].as_string());
	posX_ = screenW / buttonData[U("xDivide")].as_double();
	posY_ = screenH / buttonData[U("yDivide")].as_double();
	button.x = width * buttonData[U("buttonDiv_x")].as_double()+posX_;
	button.y = height * buttonData[U("buttonDiv_y")].as_double()+posY_;
	button.w = width * buttonData[U("buttonDiv_w")].as_double();
	button.h = height * buttonData[U("buttonDiv_h")].as_double();
}


bool GameButton::loadMedia(SDL_Renderer* gRender)
{
	if (!LoadFromFile(path_, gRender))
	{
		printf("Failed to load button animation texture!\n");
		return false;
	}
	int x = 0;
	int y = 0;
	for (int i = 0; i < BUTTON_CLIPS; i++)
	{
		buttonState[i] = { x, y, CLIP_W, CLIP_H };
		x += CLIP_W;
		if (i == 1){
			y += CLIP_H;
			x = 0;
		}
	}
	return true;
}

bool GameButton::isPressed(SDL_Event* Event)
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	if ((x > button.x) && (x < button.x + button.w) && 
		(y > button.y) && (y < button.y + button.h))
	{
		currentState = MOUSE_OVER;
		if(Event->type == SDL_MOUSEBUTTONDOWN)
		{
			currentState = MOUSE_CLICK;
			buttonPressed = true;
		}
		if (Event->type == SDL_MOUSEBUTTONUP && buttonPressed)
		{
			buttonPressed = false;
			return true;
		}
	}
	else currentState = MOUSE_OUT;

	return false;
}

void GameButton::renderButton(SDL_Renderer *gRender){
	//Render current frame
	render(posX_, posY_, &buttonState[currentState],0, NULL, SDL_FLIP_NONE, gRender, width, height);
}