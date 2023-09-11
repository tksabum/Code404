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

		// vector에서 각vertex의 순서는 반시계방향 순서대로 한다.
		CONVEXPOLYGONCOLLIDER_API void SetPolygon(const std::vector<Math::Vector2D>& vertexList);
		CONVEXPOLYGONCOLLIDER_API void SetRegularPolygon(int vertexcount, float sidesize);

		// 다각형을 이루는 점들의 좌표를 리턴
		CONVEXPOLYGONCOLLIDER_API std::vector<Math::Vector2D> GetPoints();

		// 무게중심을 월드좌표로 리턴
		CONVEXPOLYGONCOLLIDER_API Math::Vector2D GetCentroid();

		// 무게중심과 가장 먼 곳의 거리
		CONVEXPOLYGONCOLLIDER_API float GetFarthestLength();

		// 월드좌표와 가장 먼 곳의 거리
		CONVEXPOLYGONCOLLIDER_API float GetFarthestLengthFromWorldPosition();

		// 관성 모멘트 리턴
		CONVEXPOLYGONCOLLIDER_API float GetMomentOfInertia(float mass);

		// 내부에 있는 점인지 판단
		CONVEXPOLYGONCOLLIDER_API bool IsPointInside(Math::Vector2D point);

		CONVEXPOLYGONCOLLIDER_API Math::Vector2D GetSupportPoint(Math::Vector2D directionvector);

	private:
		std::vector<Math::Vector2D> points;

	};
}