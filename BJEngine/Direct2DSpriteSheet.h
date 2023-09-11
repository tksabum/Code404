#pragma once

#include <d2d1.h>
#pragma comment(lib, "D2D1.lib")

#include "SpriteSheet.h"

class IWICBitmapFrameDecode;

namespace BJEngine
{
	namespace GraphicsEngine
	{
		class Direct2DGraphics;
		class Direct2DDevice;

		class Direct2DSpriteSheet : public SpriteSheet
		{
		public:
			Direct2DSpriteSheet(std::wstring sprite);
			virtual void LoadSpriteSheet(Graphics* graphics) override;

			void Test(IWICBitmapFrameDecode* frame);

		private:
			ID2D1Bitmap* bitmap;

			friend Direct2DGraphics;
			friend Direct2DDevice;
		};
	}
}