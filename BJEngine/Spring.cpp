#include "Spring.h"
#include "Element.h"

#include "BJMath.h"
#include <cmath>




namespace BJEngine
{
using namespace Math;

	Spring::Spring(Element* elementA, Element* elementB) :
		A(elementA), B(elementB),
		length{}, springConstant{}, dampingForce{}
	{

	}

	Spring::~Spring()
	{

	}

	void Spring::SetProperties(float length, float k, float damping)
	{
		this->length = length;
		this->springConstant = k;
		this->dampingForce = damping;
	}

	void Spring::PhysicsUpdate(float deltaTime)
	{
		float distance{ (B->GetWorldPosition() - A->GetWorldPosition()).Size() };

		Vector2D direction_ab{ (B->GetWorldPosition() - A->GetWorldPosition()).GetUnitVector() };

		Vector2D V_ab{ B->GetVelocity() - A->GetVelocity() };

		Vector2D forceA{};
		Vector2D dampingA{};

		if (std::fabs(distance - length) < SPRING_EPSILON)
		{
			return;
		}

		forceA = direction_ab * -springConstant * (distance - length);
		dampingA = direction_ab * -dampingForce * DotProduct(V_ab, direction_ab);

		Vector2D springForce = forceA + dampingA;

		A->ApplyForce(springForce * -1.f);
		B->ApplyForce(springForce);
	}

	Vector2D Spring::GetElementAScreenPosition()
	{
		return A->Get2DScreenPosition();
	}

	Vector2D Spring::GetElementBScreenPosition()
	{
		return B->Get2DScreenPosition();
	}

#if _DEBUG
// 	void Spring::Render()
// 	{
// 
// 	}
#endif

}