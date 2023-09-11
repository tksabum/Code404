#pragma once

#ifdef SPRITESHEET_EXPORTS
#define SPRITESHEET_API __declspec(dllexport)
#else
#define SPRITESHEET_API __declspec(dllimport)
#endif

#include <string>

namespace BJEngine
{
	namespace GraphicsEngine
	{
		class Graphics;

		class SpriteSheet
		{
		public:
			SpriteSheet() = default;
			virtual ~SpriteSheet() = default;
			virtual void LoadSpriteSheet(Graphics* graphics) abstract;

		protected:
			std::wstring name;

			float sizeX;
			float sizeY;
		};
	}
}