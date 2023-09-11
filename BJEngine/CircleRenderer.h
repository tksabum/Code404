#pragma once

#ifdef CIRCLERENDERER_EXPORTS
#define CIRCLERENDERER_API __declspec(dllexport)
#else
#define CIRCLERENDERER_API __declspec(dllimport)
#endif

#include "Renderer.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Color.h"

namespace BJEngine
{
	class CircleRenderer : public Renderer
	{
	public:
		CIRCLERENDERER_API CircleRenderer();
		CIRCLERENDERER_API ~CircleRenderer();

		CIRCLERENDERER_API void Render(GraphicsEngine::Graphics* graphics);

		CIRCLERENDERER_API void SetCircle(float circleradius);

		CIRCLERENDERER_API void SetColor(GraphicsEngine::Color color);

	private:
		float circleRadius;
		GraphicsEngine::Color circleColor;
	};
}