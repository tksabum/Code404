#pragma once
#include "Vector2D.h"


namespace BJEngine
{
	class Element;

	constexpr float SPRING_EPSILON = 0.001f;

	class Spring
	{
	public:
		Spring(Element* elementA, Element* elementB);
		~Spring();

		void SetProperties(float length, float k, float damping);
		void PhysicsUpdate(float deltaTime);

		Math::Vector2D GetElementAScreenPosition();
		Math::Vector2D GetElementBScreenPosition();

	private:
		Element* A;
		Element* B;

		float length;
		float springConstant;
		float dampingForce;
	};
}


