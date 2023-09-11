#pragma once

#ifdef CONVEXPOLYGONCOLLIDER_EXPORTS
#define CONVEXPOLYGONCOLLIDER_API __declspec(dllexport)
#else
#define CONVEXPOLYGONCOLLIDER_API __declspec(dllimport)
#endif

#include <vector>

#include "Collider.h"

#include "Vector2D.h"
#include "Vector3D.h"

namespace BJEngine
{
	class ConvexpolygonCollider : public Collider
	{
	public:
		CONVEXPOLYGONCOLLIDER_API ConvexpolygonCollider();
		CONVEXPOLYGONCOLLIDER_API virtual ~ConvexpolygonCollider();

		// vector���� ��vertex�� ������ �ݽð���� ������� �Ѵ�.
		CONVEXPOLYGONCOLLIDER_API void SetPolygon(const std::vector<Math::Vector2D>& vertexList);
		CONVEXPOLYGONCOLLIDER_API void SetRegularPolygon(int vertexcount, float sidesize);

		// �ٰ����� �̷�� ������ ��ǥ�� ����
		CONVEXPOLYGONCOLLIDER_API std::vector<Math::Vector2D> GetPoints();

		// �����߽��� ������ǥ�� ����
		CONVEXPOLYGONCOLLIDER_API Math::Vector2D GetCentroid();

		// �����߽ɰ� ���� �� ���� �Ÿ�
		CONVEXPOLYGONCOLLIDER_API float GetFarthestLength();

		// ������ǥ�� ���� �� ���� �Ÿ�
		CONVEXPOLYGONCOLLIDER_API float GetFarthestLengthFromWorldPosition();

		// ���� ���Ʈ ����
		CONVEXPOLYGONCOLLIDER_API float GetMomentOfInertia(float mass);

		// ���ο� �ִ� ������ �Ǵ�
		CONVEXPOLYGONCOLLIDER_API bool IsPointInside(Math::Vector2D point);

		CONVEXPOLYGONCOLLIDER_API Math::Vector2D GetSupportPoint(Math::Vector2D directionvector);

	private:
		std::vector<Math::Vector2D> points;

	};
}