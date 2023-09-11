#include "Rigidbody2D.h"

#include "PhysicsSystem.h"
#include "BJMath.h"
#include "GameObject.h"
#include "Transform.h"

namespace BJEngine
{
	using namespace Math;

	const float Rigidbody2D::MASS_DEFAULT = 100.0f;
	const float Rigidbody2D::MASS_MIN = 0.000001f;
	const float Rigidbody2D::RESTITUTION_VALUE_DEFAULT = 1.0f;
	const float Rigidbody2D::RESTITUTION_VALUE_MIN = 0.0f;
	const float Rigidbody2D::RESTITUTION_VALUE_MAX = 1.0f;
	const Vector2D Rigidbody2D::GRAVITY_DEFAULT = Vector2D(0.0f, 10.0f);

	Rigidbody2D::Rigidbody2D() : velocity(Vector2D(0.0f, 0.0f)), rotationalVelocity(0.0f), mass(MASS_DEFAULT), restitutionValue(RESTITUTION_VALUE_DEFAULT), affectedByGravity(true), positionXLocked(false), positionYLocked(false), rotationLocked(false), gravity(GRAVITY_DEFAULT)
	{
		PhysicsSystem::GetInstance().AddRigidBody2D(this);
	}

	Rigidbody2D::~Rigidbody2D()
	{
		PhysicsSystem::GetInstance().RemoveRigidBody2D(this);
	}

	float Rigidbody2D::GetMagnitude()
	{
		return velocity.Size();
	}

	Vector2D Rigidbody2D::GetVelocity()
	{
		return velocity;
	}

	void Rigidbody2D::SetVelocity(Math::Vector2D v)
	{
		velocity = v;
	}

	float Rigidbody2D::GetRotationalVelocity()
	{
		return rotationalVelocity;
	}

	void Rigidbody2D::SetRotationalVelocity(float speed)
	{
		rotationalVelocity = speed;
	}

	float Rigidbody2D::GetMass()
	{
		return mass;
	}

	void Rigidbody2D::SetMass(float m)
	{
		mass = m;
		if (mass <= MASS_MIN)
		{
			mass = MASS_MIN;
		}
	}

	float Rigidbody2D::GetRestitutionValue()
	{
		return restitutionValue;
	}

	void Rigidbody2D::SetRestitutionValue(float value)
	{
		if (value < RESTITUTION_VALUE_MIN)
		{
			restitutionValue = RESTITUTION_VALUE_MIN;
		}
		else if (RESTITUTION_VALUE_MAX < value)
		{
			restitutionValue = RESTITUTION_VALUE_MAX;
		}
		else
		{
			restitutionValue = value;
		}
	}

	bool Rigidbody2D::GetPositionXLock()
	{
		return positionXLocked;
	}

	void Rigidbody2D::SetPositionXLock(bool lock)
	{
		positionXLocked = lock;
	}

	bool Rigidbody2D::GetPositionYLock()
	{
		return positionYLocked;
	}

	void Rigidbody2D::SetPositionYLock(bool lock)
	{
		positionYLocked = lock;
	}

	bool Rigidbody2D::GetRotationLock()
	{
		return rotationLocked;
	}

	void Rigidbody2D::SetRotationLock(bool lock)
	{
		rotationLocked = lock;
	}
	
	void Rigidbody2D::ApplyForce(Vector2D force)
	{
		velocity = velocity + force;
	}

	void Rigidbody2D::ApplyTorqueForce(float force)
	{
		rotationalVelocity += force;
	}

	Vector2D Rigidbody2D::GetGravity()
	{
		return gravity;
	}

	Vector2D Rigidbody2D::GetVelocityAtPoint(Vector2D point, Vector2D centroid, float inertia)
	{
		return (CrossProduct(Vector3D(0.0f, 0.0f, rotationalVelocity), point - centroid).GetUnitVector() * abs(rotationalVelocity) /** inertia / mass*/) + velocity;
	}
	
	void Rigidbody2D::SetGravity(Math::Vector2D gravityvector)
	{
		gravity = gravityvector;
	}

	bool Rigidbody2D::GetAffectedByGravity()
	{
		return affectedByGravity;
	}

	void Rigidbody2D::SetAffectedByGravity(bool affectedbygravity)
	{
		affectedByGravity = affectedbygravity;
	}
}