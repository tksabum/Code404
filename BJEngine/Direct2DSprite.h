#pragma once

#ifdef DIRECT2DSPRITE_EXPORTS
#define DIRECT2DSPRITE_API __declspec(dllexport)
#else
#define DIRECT2DSPRITE_API __declspec(dllimport)
#endif

#include "Sprite.h"

namespace BJEngine
{
	namespace GraphicsEngine
	{
		class Direct2DGraphics;
		class Direct2DDevice;

		class Direct2DSprite : public Sprite
		{
		public:
			virtual void Cut(SpriteSheet* targetsheet, int orgPosX, int orgPosY, int orgSizeX, int orgSizeY, int cutSizeX, int cutSizeY, int spaceX, int spaceY) override;

		private:

			friend Direct2DGraphics;
			friend Direct2DDevice;
		};
	}
}