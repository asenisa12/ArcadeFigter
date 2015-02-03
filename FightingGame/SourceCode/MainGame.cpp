#include "MainGame.h"

inline bool greater_than::operator() (GameCharacter* struct1, GameCharacter* struct2)
{
	return (struct1->getBottomY()< struct2->getBottomY());
}

