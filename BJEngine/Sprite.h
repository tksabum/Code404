#pragma once

#ifdef SPRITE_EXPORTS
#define SPRITE_API __declspec(dllexport)
#else
#define SPRITE_API __declspec(dllimport)
#endif

#include <string>

namespace BJEngine
{
	namespace GraphicsEngine
	{
		class SpriteSheet;

		class Sprite
		{
		public:
			virtual ~Sprite() = default;
			virtual void Cut(SpriteSheet* targetsheet, int orgPosX, int orgPosY, int orgSizeX, int orgSizeY, int cutSizeX, int cutSizeY, int spaceX, int spaceY) abstract;
			
			int GetCountX();
			int GetCountY();

			float GetCutSizeX();
			float GetCutSizeY();

		protected:
			SpriteSheet* targetSheet;

			std::wstring name;
			int countX;
			int countY;

			int orgPosX;
			int orgPosY;
			int orgSizeX;
			int orgSizeY;

			int cutSizeX;
			int cutSizeY;
			int spaceX;
			int spaceY;

		};
	}
}