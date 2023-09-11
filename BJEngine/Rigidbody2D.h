#pragma once

#ifdef RIGIDBODY2D_EXPORTS
#define RIGIDBODY2D_API __declspec(dllexport)
#else
#define RIGIDBODY2D_API __declspec(dllimport)
#endif

#include "Component.h"
#include "Vector2D.h"
#include "Vector3D.h"

namespace BJEngine
{
	class Rigidbody2D : public Component
	{
	public:
		RIGIDBODY2D_API Rigidbody2D();
		RIGIDBODY2D_API virtual ~Rigidbody2D();

		RIGIDBODY2D_API float GetMagnitude();

		RIGIDBODY2D_API Math::Vector2D GetVelocity();
		RIGIDBODY2D_API void SetVelocity(Math::Vector2D v);

		RIGIDBODY2D_API float GetRotationalVelocity();
		RIGIDBODY2D_API void SetRotationalVelocity(float speed);

		RIGIDBODY2D_API float GetMass();
		RIGIDBODY2D_API void SetMass(float m);

		RIGIDBODY2D_API float GetRestitutionValue();
		RIGIDBODY2D_API void SetRestitutionValue(float value);

		RIGIDBODY2D_API bool GetPositionXLock();
		RIGIDBODY2D_API void SetPositionXLock(bool lock);
		
		RIGIDBODY2D_API bool GetPositionYLock();
		RIGIDBODY2D_API void SetPositionYLock(bool lock);

		RIGIDBODY2D_API bool GetRotationLock();
		RIGIDBODY2D_API void SetRotationLock(bool lock);

		RIGIDBODY2D_API void ApplyForce(Math::Vector2D force);
		RIGIDBODY2D_API void ApplyTorqueForce(float force);

		RIGIDBODY2D_API Math::Vector2D GetVelocityAtPoint(Math::Vector2D point, Math::Vector2D centroid, float inertia);

		RIGIDBODY2D_API Math::Vector2D GetGravity();
		RIGIDBODY2D_API void SetGravity(Math::Vector2D gravityvector);

		RIGIDBODY2D_API bool GetAffectedByGravity();
		RIGIDBODY2D_API void SetAffectedByGravity(bool affectedbygravity);
		
	private:
		Math::Vector2D velocity;
		float rotationalVelocity;
		float mass;
		float restitutionValue;

		Math::Vector2D gravity;
		bool affectedByGravity;

		bool positionXLocked;
		bool positionYLocked;
		bool rotationLocked;

		const static float MASS_DEFAULT;
		const static float MASS_MIN;
		const static float RESTITUTION_VALUE_DEFAULT;
		const static float RESTITUTION_VALUE_MIN;
		const static float RESTITUTION_VALUE_MAX;
		const static Math::Vector2D GRAVITY_DEFAULT;
	};
}