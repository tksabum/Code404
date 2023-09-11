#pragma once

#ifdef IMAGE_EXPORTS
#define IMAGE_API __declspec(dllexport)
#else
#define IMAGE_API __declspec(dllimport)
#endif

#include <string>

#include "UIRenderer.h"

namespace BJEngine
{
	namespace GraphicsEngine
	{
		class Sprite;
		class Graphics;
	}

	class Image : public UIRenderer
	{
	public:
		IMAGE_API Image();
		IMAGE_API ~Image();

		IMAGE_API void Render(GraphicsEngine::Graphics* graphics);

		IMAGE_API void SetSpriteName(std::wstring spritename);
		IMAGE_API GraphicsEngine::Sprite* GetSprite();

		IMAGE_API void SetImageNumber(int imagenumber);
		IMAGE_API int GetImageNumber();

		IMAGE_API bool GetFlipX();
		IMAGE_API void SetFlipX(bool flipx);

		IMAGE_API bool GetFlipY();
		IMAGE_API void SetFlipY(bool flipy);

		IMAGE_API void SetOpacity(float opacity);
		IMAGE_API float GetOpacity();

	private:
		std::wstring spriteName;
		int imageNumber;
		float imageOpacity;

		bool flipX;
		bool flipY;
	};
}