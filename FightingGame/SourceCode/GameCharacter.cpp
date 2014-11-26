#include "GameCharacter.h"

void GameCharacter::renderCharacter(SDL_Renderer* gRenderer)
{
	//Render current frame
	render(posX_, posY_, currentClip, 0, NULL, flipType, gRenderer, mWidth, mHigth);
}