#include "LineRenderer.h"

#include "Graphics.h"
#include "Transform.h"
#include "GameObject.h"

#include "Matrix3x3.h"
#include "Matrix3x1.h"

namespace BJEngine
{
	using namespace Math;
	using namespace GraphicsEngine;

	LineRenderer::LineRenderer() : point1(Vector2D(0.0f, 0.0f)), point2(Vector2D(0.0f, 0.0f)), lineColor(Color::Black)
	{

	}
	
	LineRenderer::~LineRenderer()
	{

	}
	
	void LineRenderer::Render(GraphicsEngine::Graphics* graphics)
	{
		Transform* transform = GetGameObject()->GetTransform();

		Vector2D screenPosition = transform->Get2DScreenPosition();
		Vector2D screenScale = transform->Get2DScreenScale();
		float screenRotation = transform->Get2DScreenRotation();

		graphics->DrawLine(point1, point2, screenPosition.x, screenPosition.y, screenScale.x, screenScale.y, screenRotation, lineColor);
	}
	
	void LineRenderer::SetLine(Math::Vector2D p1, Math::Vector2D p2)
	{
		point1 = p1;
		point2 = p2;
	}
	
	void LineRenderer::SetColor(GraphicsEngine::Color color)
	{
		lineColor = color;
	}
}