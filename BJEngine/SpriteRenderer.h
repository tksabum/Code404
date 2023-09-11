#pragma once

#ifdef SPRITERENDERER_EXPORTS
#define SPRITERENDERER_API __declspec(dllexport)
#else
#define SPRITERENDERER_API __declspec(dllimport)
#endif

#include <string>

#include "Renderer.h"

namespace BJEngine
{
	namespace GraphicsEngine
	{
		class Sprite;
		class Graphics;
	}

	class SpriteRenderer : public Renderer
	{
	public:
		SPRITERENDERER_API SpriteRenderer();
		SPRITERENDERER_API ~SpriteRenderer();

		SPRITERENDERER_API void Render(GraphicsEngine::Graphics* graphics);

		SPRITERENDERER_API void SetSpriteName(std::wstring spritename);
		SPRITERENDERER_API GraphicsEngine::Sprite* GetSprite();

		SPRITERENDERER_API void SetImageNumber(int imagenumber);
		SPRITERENDERER_API int GetImageNumber();

		SPRITERENDERER_API bool GetFlipX();
		SPRITERENDERER_API void SetFlipX(bool flipx);

		SPRITERENDERER_API bool GetFlipY();
		SPRITERENDERER_API void SetFlipY(bool flipy);

		SPRITERENDERER_API void SetOpacity(float opacity);
		SPRITERENDERER_API float GetOpacity();

	private:
		std::wstring spriteName;
		int imageNumber;
		float imageOpacity;

		bool flipX;
		bool flipY;
	};
}