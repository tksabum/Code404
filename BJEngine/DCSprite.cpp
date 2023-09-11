#include "DCSprite.h"

void BJEngine::GraphicsEngine::DCSprite::Cut(SpriteSheet* targetsheet, int orgPosX, int orgPosY, int orgSizeX, int orgSizeY, int cutSizeX, int cutSizeY, int spaceX, int spaceY)
{
	if (targetSheet != nullptr)
	{
		return;
	}

	targetSheet = targetsheet;

	this->orgPosX = orgPosX;
	this->orgPosY = orgPosY;
	this->orgSizeX = orgSizeX;
	this->orgSizeY = orgSizeY;

	this->cutSizeX = cutSizeX;
	this->cutSizeY = cutSizeY;
	this->spaceX = spaceX;
	this->spaceY = spaceY;

	countX = orgSizeX / (cutSizeX + spaceX) + (orgSizeX % (cutSizeX + spaceX)) / cutSizeX;
	countY = orgSizeY / (cutSizeY + spaceY) + (orgSizeY % (cutSizeY + spaceY)) / cutSizeY;
}
