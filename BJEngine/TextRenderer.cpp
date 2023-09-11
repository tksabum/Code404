#include "TextRenderer.h"

#include "Graphics.h"
#include "Transform.h"
#include "GameObject.h"

namespace BJEngine
{
	using namespace Math;
	using namespace GraphicsEngine;

	const int TextRenderer::DEFAULT_TEXT_SIZE = 10;

	TextRenderer::TextRenderer() : textColor(Color::Black), textSize(DEFAULT_TEXT_SIZE)
	{

	}

	TextRenderer::~TextRenderer()
	{

	}

	void BJEngine::TextRenderer::Render(GraphicsEngine::Graphics* graphics)
	{
		Transform* transform = GetGameObject()->GetTransform();

		Vector2D screenPosition = transform->Get2DScreenPosition();
		Vector2D screenScale = transform->Get2DScreenScale();
		float screenRotation = transform->Get2DScreenRotation();

		graphics->DrawString(text, screenPosition.x, screenPosition.y, textAreaSize.x, textAreaSize.y, screenScale.x, screenScale.y, screenRotation, textSize, textColor);
	}

	void BJEngine::TextRenderer::SetText(std::wstring str)
	{
		text = str;
	}

	void BJEngine::TextRenderer::SetTextSize(int size)
	{
		textSize = size;
	}

	void BJEngine::TextRenderer::SetTextColor(GraphicsEngine::Color color)
	{
		textColor = color;
	}

	void TextRenderer::SetTextAreaSize(Math::Vector2D areasize)
	{
		textAreaSize = areasize;
	}
}