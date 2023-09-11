#pragma once

#ifdef COLLIDER_EXPORTS
#define COLLIDER_API __declspec(dllexport)
#else
#define COLLIDER_API __declspec(dllimport)
#endif

#include "Component.h"
#include "Vector2D.h"

namespace BJEngine
{
	class PhysicsSystem;

	class Collider : public Component
	{
	public:
		COLLIDER_API Collider();
		COLLIDER_API virtual ~Collider();

		COLLIDER_API bool GetTrigger();
		COLLIDER_API void SetTrigger(bool trigger);

		COLLIDER_API Math::Vector2D GetLastCollisionVector();

		// 무게중심을 월드좌표로 리턴
		COLLIDER_API virtual Math::Vector2D GetCentroid() abstract;

		// 무게중심과 가장 먼 곳의 거리
		COLLIDER_API virtual float GetFarthestLength() abstract;

		// 월드좌표와 가장 먼 곳의 거리
		COLLIDER_API virtual float GetFarthestLengthFromWorldPosition() abstract;

		// 관성 모멘트 리턴
		COLLIDER_API virtual float GetMomentOfInertia(float mass) abstract;

		// 내부에 있는 점인지 판단
		COLLIDER_API virtual bool IsPointInside(Math::Vector2D point) abstract;

		// GJK 알고리즘에서 사용되는 SupportPoint를 얻는 함수
		// Direction Vector와 각 정점의 벡터의 내적값 중에서 가장 큰 값을 리턴한다.
		COLLIDER_API virtual Math::Vector2D GetSupportPoint(Math::Vector2D directionvector) abstract;

	private:
		bool isTrigger;

		// 마지막으로 충돌한 깊이와 방향
		Math::Vector2D lastCollisionVector;

		friend PhysicsSystem;
	};
}