#pragma once

#ifdef BOXCOLLIDER_EXPORTS
#define BOXCOLLIDER_API __declspec(dllexport)
#else
#define BOXCOLLIDER_API __declspec(dllimport)
#endif
#include <vector>

#include "Collider.h"

#include "Vector2D.h"
#include "Vector3D.h"

namespace BJEngine
{
	class BoxCollider : public Collider
	{
	public:
		BOXCOLLIDER_API BoxCollider();
		BOXCOLLIDER_API virtual ~BoxCollider();

		// vector���� ��vertex�� ������ �ݽð���� ������� �Ѵ�.
		BOXCOLLIDER_API void SetRectangle(float vertexcount, float sidesize);

		// �ٰ����� �̷�� ������ ��ǥ�� ����
		BOXCOLLIDER_API std::vector<Math::Vector2D> GetPoints();

		BOXCOLLIDER_API Math::Vector2D GetCentroid();

		BOXCOLLIDER_API float GetFarthestLength();

		BOXCOLLIDER_API float GetFarthestLengthFromWorldPosition();

		BOXCOLLIDER_API float GetMomentOfInertia(float mass);

		BOXCOLLIDER_API bool IsPointInside(Math::Vector2D point);

		BOXCOLLIDER_API Math::Vector2D GetSupportPoint(Math::Vector2D directionvector);

	private:
		std::vector<Math::Vector2D> points;
	};
}