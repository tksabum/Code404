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

		// �������� ��ī��Ʈ ��ǥ�踦 �������� �ݽð� ���� ������� �Է��Ͽ��� �Ѵ�.
		POLYGONRENDERER_API void SetPolygon(const std::vector<Math::Vector2D>& pointlist);
		// �������� ���� �Ѻ��� ũ��� ���ٰ����� �����Ѵ�.
		POLYGONRENDERER_API void SetRegularPolygon(int vertexcount, float sidesize);

		POLYGONRENDERER_API void SetColor(GraphicsEngine::Color color);

	private:
		std::vector<Math::Vector2D> pointList;
		GraphicsEngine::Color polygonColor;

	};
}