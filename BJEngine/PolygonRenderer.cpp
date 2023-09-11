#include "PolygonRenderer.h"

#include "Graphics.h"
#include "Transform.h"
#include "GameObject.h"

#include "Matrix3x3.h"
#include "Matrix3x1.h"

using namespace std;

namespace BJEngine
{
	using namespace Math;
	using namespace GraphicsEngine;

	PolygonRenderer::PolygonRenderer() : polygonColor(Color::Black)
	{

	}

	PolygonRenderer::~PolygonRenderer()
	{
		
	}
	
	void PolygonRenderer::Render(GraphicsEngine::Graphics* graphics)
	{
		Transform* transform = GetGameObject()->GetTransform();

		Vector2D screenPosition = transform->Get2DScreenPosition();
		Vector2D screenScale = transform->Get2DScreenScale();
		float screenRotation = transform->Get2DScreenRotation();

		graphics->DrawPolygon(pointList, screenPosition.x, screenPosition.y, screenScale.x, screenScale.y, screenRotation, polygonColor);
	}
	
	void PolygonRenderer::SetPolygon(const vector<Vector2D>& pointlist)
	{
		pointList = pointlist;
	}
	
	void PolygonRenderer::SetRegularPolygon(int vertexcount, float sidesize)
	{
		// �ʱ�ȭ
		pointList.clear();
		
		// �������� �����Ͽ� ���ٰ��� �׸���
		Vector2D p(0.0f, 0.0f);
		Vector2D direction(-1.0f, 0.0f);
		Matrix3x3 rotate = Matrix3x3::RotationByDegree(360.0f / vertexcount);

		for (int i = 0; i < vertexcount; i++)
		{
			pointList.push_back(p);
			p = p + (direction * sidesize);
			direction = rotate * direction;
		}

		// �����߽� ã��
		Vector2D centroid(0.0f, 0.0f);

		for (int i = 0; i < pointList.size(); i++)
		{
			centroid = centroid + pointList[i];
		}

		centroid = centroid / pointList.size();

		// �����߽��� ������ �ǵ��� �̵�
		for (int i = 0; i < pointList.size(); i++)
		{
			pointList[i] = pointList[i] - centroid;
		}
	}

	void PolygonRenderer::SetColor(GraphicsEngine::Color color)
	{
		polygonColor = color;
	}
}