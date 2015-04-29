#include "GameButton.h"

GameButton::GameButton(int screenW, int screenH, Value& buttonData)
	:framesToEnd(0), buttonPressed(false)
{
	width = screenW / buttonData["widthDivide"].GetInt();
	height = screenH / buttonData["heightDivide"].GetInt();
	CLIP_W = buttonData["CLIP_W"].GetInt();
	CLIP_H = buttonData["CLIP_H"].GetInt();
	path_ = buttonData["texture"].GetString();
	posX_ = screenW / buttonData["xDivide"].GetDouble();
	posY_ = screenH / buttonData["yDivide"].GetDouble();
	button.x = width * buttonData["buttonDiv_x"].GetDouble()+posX_;
	button.y = height * buttonData["buttonDiv_y"].GetDouble()+posY_;
	button.w = width * buttonData["buttonDiv_w"].GetDouble();
	button.h = height * buttonData["buttonDiv_h"].GetDouble();
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