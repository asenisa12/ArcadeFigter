#include "GameButton.h"

GameButton::GameButton(int screenW, int screenH, int posX, int posY)
	:width(screenW / 6), height(screenH / 6), framesToEnd(0)
{
	posX_ = posX; 
	posY_ = posY;
}

bool GameButton::loadMedia(std::string path, SDL_Renderer* gRender)
{
	if (!LoadFromFile(path, gRender))
	{
		printf("Failed to load button animation texture!\n");
		return false;
	}
	int x = 0;
	int y = 0;
	for (int i = 0; i < BUTTON_CLIPS; i++)
	{
		buttonState[i].x = x;
		buttonState[i].y = y;
		buttonState[i].w = CLIP_W;
		buttonState[i].h = CLIP_H;
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
	if (Event->type == SDL_MOUSEMOTION || Event->type == SDL_MOUSEBUTTONDOWN)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		bool inside = true;

		//Mouse is left of the button
		if (x < posX_)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (x > posX_ + width)
		{
			inside = false;
		}
		//Mouse above the button
		else if (y < posY_)
		{
			inside = false;
		}
		//Mouse below the button
		else if (y > posY_ + height)
		{
			inside = false;
		}
		if (inside)
		{
			currentState = MOUSE_OVER;
			if(Event->type == SDL_MOUSEBUTTONDOWN)
			{
				currentState = MOUSE_CLICK;
				framesToEnd++;
				if (framesToEnd > 5){
					framesToEnd = 0;
					return true;
				}
			}
		}
		else currentState = MOUSE_OUT;
	
	}
	return false;
}

void GameButton::renderButton(SDL_Renderer *gRender){
	//Render current frame
	render(posX_, posY_, &buttonState[currentState],0, NULL, SDL_FLIP_NONE, gRender, width, height);
}