#pragma once

#ifdef CIRCLECOLLIDER_EXPORTS
#define CIRCLECOLLIDER_API __declspec(dllexport)
#else
#define CIRCLECOLLIDER_API __declspec(dllimport)
#endif

#include "Collider.h"

#include "Vector2D.h"
#include "Vector3D.h"

namespace BJEngine
{
	class CircleCollider : public Collider
	{
	public:
		CIRCLECOLLIDER_API CircleCollider();
		CIRCLECOLLIDER_API virtual ~CircleCollider();

		CIRCLECOLLIDER_API void SetCircle(float circleradius);

		// 무게중심을 월드좌표로 리턴
		CIRCLECOLLIDER_API Math::Vector2D GetCentroid();

		// 무게중심과 가장 먼 곳의 거리
		CIRCLECOLLIDER_API float GetFarthestLength();

		// 월드좌표와 가장 먼 곳의 거리
		CIRCLECOLLIDER_API float GetFarthestLengthFromWorldPosition();

		// 관성 모멘트 리턴
		CIRCLECOLLIDER_API float GetMomentOfInertia(float mass);

		CIRCLECOLLIDER_API float GetRadius();

		// 내부에 있는 점인지 판단
		CIRCLECOLLIDER_API bool IsPointInside(Math::Vector2D point);

		CIRCLECOLLIDER_API Math::Vector2D GetSupportPoint(Math::Vector2D directionvector);

	private:
		float radius;

	};
}


