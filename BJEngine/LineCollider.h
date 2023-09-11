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

		// �����߽��� ������ǥ�� ����
		LINECOLLIDER_API Math::Vector2D GetCentroid();

		// �����߽ɰ� ���� �� ���� �Ÿ�
		LINECOLLIDER_API float GetFarthestLength();

		// ������ǥ�� ���� �� ���� �Ÿ�
		LINECOLLIDER_API float GetFarthestLengthFromWorldPosition();

		LINECOLLIDER_API Math::Vector2D GetSupportPoint(Math::Vector2D directionvector);

		LINECOLLIDER_API Math::Vector2D GetPoint1();
		LINECOLLIDER_API Math::Vector2D GetPoint2();

		// ���� ���Ʈ ����
		LINECOLLIDER_API float GetMomentOfInertia(float mass);

		// ���ο� �ִ� ������ �Ǵ�
		LINECOLLIDER_API bool IsPointInside(Math::Vector2D point);

	private:
		Math::Vector2D point1;
		Math::Vector2D point2;

	};
}