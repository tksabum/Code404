#pragma once

#ifdef LINERENDERER_EXPORTS
#define LINERENDERER_API __declspec(dllexport)
#else
#define LINERENDERER_API __declspec(dllimport)
#endif

#include "Renderer.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Color.h"

namespace BJEngine
{
	namespace GraphicsEngine
	{
		class Graphics;
	}

	class LineRenderer : public Renderer
	{
	public:
		LINERENDERER_API LineRenderer();
		LINERENDERER_API ~LineRenderer();

		LINERENDERER_API void Render(GraphicsEngine::Graphics* graphics);

		LINERENDERER_API void SetLine(Math::Vector2D p1, Math::Vector2D p2);

		LINERENDERER_API void SetColor(GraphicsEngine::Color color);

	private:
		Math::Vector2D point1;
		Math::Vector2D point2;
		GraphicsEngine::Color lineColor;

	};
}
