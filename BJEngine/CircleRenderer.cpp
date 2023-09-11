#include "CircleRenderer.h"

#include "GameObject.h"
#include "Transform.h"
#include "Graphics.h"

namespace BJEngine
{
	using namespace GraphicsEngine;
	using namespace Math;

	CircleRenderer::CircleRenderer() : circleColor(Color::Black), circleRadius(0.0f)
	{

	}

	CircleRenderer::~CircleRenderer()
	{

	}

	void CircleRenderer::Render(Graphics* graphics)
	{
		Transform* transform = GetGameObject()->GetTransform();

		Vector2D screenPosition = transform->Get2DScreenPosition();
		Vector2D screenScale = transform->Get2DScreenScale();
		float screenRotation = transform->Get2DScreenRotation();

		graphics->DrawEllipse(screenPosition.x, screenPosition.y, circleRadius, circleRadius, screenScale.x, screenScale.y, screenRotation, circleColor);
	}

	void CircleRenderer::SetCircle(float circleradius)
	{
		circleRadius = circleradius;
	}

	void CircleRenderer::SetColor(Color color)
	{
		circleColor = color;
	}
}