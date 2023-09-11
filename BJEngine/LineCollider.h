#pragma once

#ifdef LINECOLLIDER_EXPORTS
#define LINECOLLIDER_API __declspec(dllexport)
#else
#define LINECOLLIDER_API __declspec(dllimport)
#endif

#include "Collider.h"

#include "Vector2D.h"
#include "Vector3D.h"

namespace BJEngine
{
	class LineCollider : public Collider
	{
	public:
		LINECOLLIDER_API LineCollider();
		LINECOLLIDER_API virtual ~LineCollider();
		
		LINECOLLIDER_API void SetLine(Math::Vector2D p1, Math::Vector2D p2);

		// 무게중심을 월드좌표로 리턴
		LINECOLLIDER_API Math::Vector2D GetCentroid();

		// 무게중심과 가장 먼 곳의 거리
		LINECOLLIDER_API float GetFarthestLength();

		// 월드좌표와 가장 먼 곳의 거리
		LINECOLLIDER_API float GetFarthestLengthFromWorldPosition();

		LINECOLLIDER_API Math::Vector2D GetSupportPoint(Math::Vector2D directionvector);

		LINECOLLIDER_API Math::Vector2D GetPoint1();
		LINECOLLIDER_API Math::Vector2D GetPoint2();

		// 관성 모멘트 리턴
		LINECOLLIDER_API float GetMomentOfInertia(float mass);

		// 내부에 있는 점인지 판단
		LINECOLLIDER_API bool IsPointInside(Math::Vector2D point);

	private:
		Math::Vector2D point1;
		Math::Vector2D point2;

	};
}