#include "Collider.h"

#include "PhysicsSystem.h"
#include "NavigationSystem.h"

namespace BJEngine
{
	Collider::Collider() : isTrigger(false)
	{
		PhysicsSystem::GetInstance().AddCollider(this);
		NavigationSystem::GetInstance().AddCollider(this);
	}

	Collider::~Collider()
	{
		PhysicsSystem::GetInstance().RemoveCollider(this);
		NavigationSystem::GetInstance().RemoveCollider(this);
	}

	bool Collider::GetTrigger()
	{
		return isTrigger;
	}

	void Collider::SetTrigger(bool trigger)
	{
		isTrigger = trigger;
	}

	Math::Vector2D Collider::GetLastCollisionVector()
	{
		return lastCollisionVector;
	}
}