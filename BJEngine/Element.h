#pragma once

#ifdef ELEMENT_EXPORTS
#define ELEMENT_API __declspec(dllexport)
#else
#define ELEMENT_API __declspec(dllimport)
#endif

#include "Vector2D.h"

namespace BJEngine
{

	class SoftBody;

	class Element
	{
	public:
		ELEMENT_API Element(SoftBody* parent);
		ELEMENT_API ~Element();

		ELEMENT_API void ApplyForce(Math::Vector2D force);
		ELEMENT_API void RemoveForce();
		ELEMENT_API void ForceToVelocity(Math::Vector2D gravity, float deltaTime);
		ELEMENT_API Math::Vector2D PositionUpdate(float deltaTime);

		ELEMENT_API void MovePosition(Math::Vector2D pos);
		
		ELEMENT_API void SetPosition(Math::Vector2D pos);
		ELEMENT_API void SetVelocity(Math::Vector2D velo);
		ELEMENT_API void SetAccelerator(Math::Vector2D accel);
		ELEMENT_API void SetRadius(float r);
		ELEMENT_API void SetMass(float m);
		ELEMENT_API void SetRestitutionValue(float e);

		ELEMENT_API Math::Vector2D GetPosition();
		ELEMENT_API Math::Vector2D Get2DScreenPosition();
		ELEMENT_API Math::Vector2D Get2DScreenScale();
		ELEMENT_API float Get2DScreenRotation();
		ELEMENT_API Math::Vector2D GetWorldPosition();
		ELEMENT_API Math::Vector2D GetWorldScale();
		ELEMENT_API float GetWorldRotation();
		ELEMENT_API Math::Vector2D GetVelocity();
		ELEMENT_API float GetRadius();
		ELEMENT_API float GetMass();
		ELEMENT_API float GetRestitutionValue();

	private:
		SoftBody* parent;

		Math::Vector2D Force{};

		Math::Vector2D position{};
		Math::Vector2D velocity{};
		Math::Vector2D accelerator{};
		
		float radius;
		float mass;
		float restitution;
	};
}


/*
element 

soft body의 position으로 부터 상대 좌표를 가지고 있어야 할 것 같다.

\
*/