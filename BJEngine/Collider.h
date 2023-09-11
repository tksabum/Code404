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

		// �����߽��� ������ǥ�� ����
		COLLIDER_API virtual Math::Vector2D GetCentroid() abstract;

		// �����߽ɰ� ���� �� ���� �Ÿ�
		COLLIDER_API virtual float GetFarthestLength() abstract;

		// ������ǥ�� ���� �� ���� �Ÿ�
		COLLIDER_API virtual float GetFarthestLengthFromWorldPosition() abstract;

		// ���� ���Ʈ ����
		COLLIDER_API virtual float GetMomentOfInertia(float mass) abstract;

		// ���ο� �ִ� ������ �Ǵ�
		COLLIDER_API virtual bool IsPointInside(Math::Vector2D point) abstract;

		// GJK �˰��򿡼� ���Ǵ� SupportPoint�� ��� �Լ�
		// Direction Vector�� �� ������ ������ ������ �߿��� ���� ū ���� �����Ѵ�.
		COLLIDER_API virtual Math::Vector2D GetSupportPoint(Math::Vector2D directionvector) abstract;

	private:
		bool isTrigger;

		// ���������� �浹�� ���̿� ����
		Math::Vector2D lastCollisionVector;

		friend PhysicsSystem;
	};
}