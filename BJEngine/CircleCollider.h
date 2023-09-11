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

		// �����߽��� ������ǥ�� ����
		CIRCLECOLLIDER_API Math::Vector2D GetCentroid();

		// �����߽ɰ� ���� �� ���� �Ÿ�
		CIRCLECOLLIDER_API float GetFarthestLength();

		// ������ǥ�� ���� �� ���� �Ÿ�
		CIRCLECOLLIDER_API float GetFarthestLengthFromWorldPosition();

		// ���� ���Ʈ ����
		CIRCLECOLLIDER_API float GetMomentOfInertia(float mass);

		CIRCLECOLLIDER_API float GetRadius();

		// ���ο� �ִ� ������ �Ǵ�
		CIRCLECOLLIDER_API bool IsPointInside(Math::Vector2D point);

		CIRCLECOLLIDER_API Math::Vector2D GetSupportPoint(Math::Vector2D directionvector);

	private:
		float radius;

	};
}


