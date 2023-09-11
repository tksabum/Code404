#pragma once

#ifdef TEXTRENDERER_EXPORTS
#define TEXTRENDERER_API __declspec(dllexport)
#else
#define TEXTRENDERER_API __declspec(dllimport)
#endif

#include <string>

#include "Color.h"
#include "Renderer.h"
#include "Vector2D.h"

namespace BJEngine
{
	namespace GraphicsEngine
	{
		class Graphics;
	}

	class TextRenderer : public Renderer
	{
	public:
		TEXTRENDERER_API TextRenderer();
		TEXTRENDERER_API ~TextRenderer();

		TEXTRENDERER_API void Render(GraphicsEngine::Graphics* graphics);

		TEXTRENDERER_API void SetText(std::wstring str);
		TEXTRENDERER_API void SetTextSize(int size);
		TEXTRENDERER_API void SetTextColor(GraphicsEngine::Color color);

		TEXTRENDERER_API void SetTextAreaSize(Math::Vector2D areasize);

	private:
		std::wstring text;
		int textSize;
		GraphicsEngine::Color textColor;
		Math::Vector2D textAreaSize;

		const static int DEFAULT_TEXT_SIZE;
	};
}