#pragma once
#include "Sprite.h"

namespace BJEngine
{
    namespace GraphicsEngine
    {
		class Direct2DDevice;

		class DCSprite :
			public Sprite
		{
		public:
			virtual void Cut(SpriteSheet* targetsheet, int orgPosX, int orgPosY, int orgSizeX, int orgSizeY, int cutSizeX, int cutSizeY, int spaceX, int spaceY) override;

			friend Direct2DDevice;
		};
    }
}



