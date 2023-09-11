#pragma once

#ifdef SOFTBODY2D_EXPORTS
#define SOFTBODY2D_API __declspec(dllexport)
#else
#define SOFTBODY2D_API __declspec(dllimport)
#endif

#include "Component.h"

#include <vector>

#include "Vector2D.h"

namespace BJEngine
{
	class ConvexpolygonCollider;

	// �������� ���� ���� �������� ���� ���ٰ��� Collider�� �Բ� ����ؾ��Ѵ�.
	class Softbody2D : public Component
	{
	public:
		SOFTBODY2D_API Softbody2D();
		SOFTBODY2D_API ~Softbody2D();
		SOFTBODY2D_API void Awake();

	private:
		ConvexpolygonCollider* collider;

		Math::Vector2D velocity;
		float rotationalVelocity;
		float mass;
		float restitutionValue;

		Math::Vector2D gravity;
		bool affectedByGravity;

		bool positionXLocked;
		bool positionYLocked;
		bool rotationLocked;

		// ����� ����
		int depth;
		// �������� ��
		int vertexCount;
		// �ε巯�� ���� (0.1 ~ 1.0)
		float softnessFactor;

		// depth, nodenumber
		std::vector<std::vector<Math::Vector2D>> orgNodes;

		std::vector<std::vector<float>> orgVerticalSpring;
		std::vector<std::vector<float>> orgHorizontalSpring;

		std::vector<std::vector<float>> verticalSpringRepulsiveForce;
		std::vector<std::vector<float>> horizontalSpringRepulsiveForce;

		const static float MASS_DEFAULT;
		const static float MASS_MIN;
		const static float RESTITUTION_VALUE_DEFAULT;
		const static float RESTITUTION_VALUE_MIN;
		const static float RESTITUTION_VALUE_MAX;
		const static Math::Vector2D GRAVITY_DEFAULT;

		const static int DEPTH_DEFAULT;
		const static float SOFTNESS_FACTOR_DEFAULT;
	};
}