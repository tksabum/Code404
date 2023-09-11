#include "Text.h"

#include "Graphics.h"
#include "Transform.h"
#include "GameObject.h"

namespace BJEngine
{
	using namespace Math;
	using namespace GraphicsEngine;

	const int Text::DEFAULT_TEXT_SIZE = 10;

	Text::Text() : textColor(Color::Black), textSize(DEFAULT_TEXT_SIZE), font()
	{

	}

	Text::~Text()
	{

	}

	void Text::Render(GraphicsEngine::Graphics* graphics)
	{
		Transform* transform = GetGameObject()->GetTransform();

		Vector2D screenScale = transform->GetUIScale();
		Vector2D screenPosition = transform->GetUIPosition() + Vector2D((textAreaSize.x * screenScale.x) / 2.0f, (textAreaSize.y * screenScale.y) / 2.0f);
		float screenRotation = transform->GetUIRotation();

		if (font.empty())
		{
			font = L"consolas";
		}

		graphics->DrawString(text, screenPosition.x, screenPosition.y, textAreaSize.x, textAreaSize.y, screenScale.x, screenScale.y, screenRotation, textSize, textColor, font.c_str());
	}
	
	void Text::SetText(std::wstring str)
	{
		text = str;
	}
	
	void Text::SetFont(std::wstring fontname)
	{
		font = fontname;
	}

	void Text::SetTextSize(int size)
	{
		textSize = size;
	}
	
	void Text::SetTextColor(GraphicsEngine::Color color)
	{
		textColor = color;
	}
	
	void Text::SetTextAreaSize(Math::Vector2D areasize)
	{
		textAreaSize = areasize;
	}
}