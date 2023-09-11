#pragma once

#ifdef POLYGONRENDERER_EXPORTS
#define POLYGONRENDERER_API __declspec(dllexport)
#else
#define POLYGONRENDERER_API __declspec(dllimport)
#endif

#include <vector>

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

	class PolygonRenderer : public Renderer
	{
	public:
		POLYGONRENDERER_API PolygonRenderer();
		POLYGONRENDERER_API ~PolygonRenderer();

		POLYGONRENDERER_API void Render(GraphicsEngine::Graphics* graphics);

		// 꼭지점을 데카르트 좌표계를 기준으로 반시계 방향 순서대로 입력하여야 한다.
		POLYGONRENDERER_API void SetPolygon(const std::vector<Math::Vector2D>& pointlist);
		// 꼭지점의 수와 한변의 크기로 정다각형을 생성한다.
		POLYGONRENDERER_API void SetRegularPolygon(int vertexcount, float sidesize);

		POLYGONRENDERER_API void SetColor(GraphicsEngine::Color color);

	private:
		std::vector<Math::Vector2D> pointList;
		GraphicsEngine::Color polygonColor;

	};
}