#pragma once

#include <d2d1.h>
#pragma comment(lib, "D2D1.lib")

#include "SpriteSheet.h"

namespace BJEngine
{
    namespace GraphicsEngine
    {
		class Direct2DDevice;

		class DCSpriteSheet :
			public SpriteSheet
		{
		public:
			DCSpriteSheet(std::wstring sprite);
			virtual void LoadSpriteSheet(Graphics* graphics) override;

		private:
			ID2D1Bitmap* bitmap;

			friend Direct2DDevice;
		};
    }
}



