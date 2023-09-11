#include "CircleCollider.h"

#include "GameObject.h"
#include "Transform.h"
#include "Matrix3x1.h"
#include "Matrix3x3.h"

namespace BJEngine
{
	using namespace Math;

	CircleCollider::CircleCollider() : radius(0.0f)
	{

	}

	CircleCollider::~CircleCollider()
	{

	}

	void CircleCollider::SetCircle(float circleradius)
	{
		radius = circleradius;
	}

	Vector2D CircleCollider::GetCentroid()
	{
		return gameObject->GetTransform()->GetWorldPosition();
	}

	float CircleCollider::GetFarthestLength()
	{
		Vector2D gameobjectWorldScale = gameObject->GetTransform()->GetWorldScale();

		float maxscale = gameobjectWorldScale.x;

		if (maxscale < gameobjectWorldScale.y)
		{
			maxscale = gameobjectWorldScale.y;
		}

		return radius * maxscale;
	}

	float CircleCollider::GetFarthestLengthFromWorldPosition()
	{
		Vector2D gameobjectWorldScale = gameObject->GetTransform()->GetWorldScale();

		float maxscale = gameobjectWorldScale.x;

		if (maxscale < gameobjectWorldScale.y)
		{
			maxscale = gameobjectWorldScale.y;
		}

		return radius * maxscale;
	}

	float CircleCollider::GetMomentOfInertia(float mass)
	{
		Vector2D gameobjectWorldScale = gameObject->GetTransform()->GetWorldScale();

		float scaleX = gameobjectWorldScale.x;
		float scaleY = gameobjectWorldScale.y;

		float r = radius / 30.0f;
		float sx = scaleX / 30.0f;
		float sy = scaleY / 30.0f;

		return (mass * (r * r * sx * sx + r * r * sy * sy)) / 4.0f;

		//return mass;
	}

	float CircleCollider::GetRadius()
	{
		return radius;
	}

	bool CircleCollider::IsPointInside(Math::Vector2D point)
	{
		Vector2D gameobjectWorldPosition = gameObject->GetTransform()->GetWorldPosition();
		Vector2D gameobjectWorldScale = gameObject->GetTransform()->GetWorldScale();
		float gameobjectWorldRotation = gameObject->GetTransform()->GetWorldRotation().z;

		Matrix3x3 transformMatrix = Matrix3x3::Translation(gameobjectWorldPosition) * Matrix3x3::RotationByDegree(gameobjectWorldRotation);

		if (gameobjectWorldScale.x == 0.0f || gameobjectWorldScale.y == 0.0f)
		{
			return false;
		}

		Vector2D localpoint = Matrix3x3::Scale(Vector2D(1.0f / gameobjectWorldScale.x, 1.0f / gameobjectWorldScale.y)) * Matrix3x3::RotationByDegree(-gameobjectWorldRotation) * Matrix3x3::Translation(-gameobjectWorldPosition) * point;

		if (localpoint.x * localpoint.x + localpoint.y * localpoint.y <= radius * radius)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	Vector2D CircleCollider::GetSupportPoint(Vector2D directionvector)
	{
		Vector2D gameobjectWorldPosition = gameObject->GetTransform()->GetWorldPosition();
		Vector2D gameobjectWorldScale = gameObject->GetTransform()->GetWorldScale();
		float gameobjectWorldRotation = gameObject->GetTransform()->GetWorldRotation().z;

		Matrix3x3 transformMatrix = Matrix3x3::Translation(gameobjectWorldPosition) * Matrix3x3::RotationByDegree(gameobjectWorldRotation);

		Vector2D localsupportpoint = Matrix3x3::Scale(gameobjectWorldScale) * (((Vector2D)(Matrix3x3::Scale(gameobjectWorldScale) * Matrix3x3::RotationByDegree(-gameobjectWorldRotation) * directionvector)).GetUnitVector() * radius);

		return transformMatrix * localsupportpoint;
	}
}