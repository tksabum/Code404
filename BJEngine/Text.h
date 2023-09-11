#pragma once

#ifdef TEXT_EXPORTS
#define TEXT_API __declspec(dllexport)
#else
#define TEXT_API __declspec(dllimport)
#endif

#include "UIRenderer.h"

#include <string>

#include "Color.h"
#include "Vector2D.h"

namespace BJEngine
{
	namespace GraphicsEngine
	{
		class Graphics;
	}

	class Text : public UIRenderer
	{
	public:
		TEXT_API Text();
		TEXT_API ~Text();

		TEXT_API void Render(GraphicsEngine::Graphics* graphics);

		TEXT_API void SetText(std::wstring str);
		TEXT_API void SetFont(std::wstring fontname);
		TEXT_API void SetTextSize(int size);
		TEXT_API void SetTextColor(GraphicsEngine::Color color);

		TEXT_API void SetTextAreaSize(Math::Vector2D areasize);

	private:
		std::wstring text;
		std::wstring font;
		int textSize;
		GraphicsEngine::Color textColor;
		Math::Vector2D textAreaSize;

		const static int DEFAULT_TEXT_SIZE;
	};
}