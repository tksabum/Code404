#include "LineCollider.h"

#include "GameObject.h"
#include "Transform.h"
#include "BJMath.h"
#include "Matrix3x1.h"
#include "Matrix3x3.h"

namespace BJEngine
{
	using namespace Math;

	LineCollider::LineCollider() : point1(Vector2D(0.0f, 0.0f)), point2(Vector2D(0.0f, 0.0f))
	{

	}

	LineCollider::~LineCollider()
	{

	}

	void LineCollider::SetLine(Math::Vector2D p1, Math::Vector2D p2)
	{
		point1 = p1;
		point2 = p2;
	}

	Vector2D LineCollider::GetCentroid()
	{
		Vector2D localCentroid = (point1 + point2) / 2.0f;

		Vector2D gameobjectWorldPosition = gameObject->GetTransform()->GetWorldPosition();
		Vector2D gameobjectWorldScale = gameObject->GetTransform()->GetWorldScale();
		float gameobjectWorldRotation = gameObject->GetTransform()->GetWorldRotation().z;

		Matrix3x3 transformMatrix = Matrix3x3::Translation(gameobjectWorldPosition) * Matrix3x3::RotationByDegree(gameobjectWorldRotation) * Matrix3x3::Scale(gameobjectWorldScale);

		return transformMatrix * localCentroid;
	}

	float LineCollider::GetFarthestLength()
	{
		Vector2D localCentroid = (point1 + point2) / 2.0f;

		Vector2D gameobjectWorldPosition = gameObject->GetTransform()->GetWorldPosition();
		Vector2D gameobjectWorldScale = gameObject->GetTransform()->GetWorldScale();
		float gameobjectWorldRotation = gameObject->GetTransform()->GetWorldRotation().z;

		Matrix3x3 transformMatrix = Matrix3x3::Translation(gameobjectWorldPosition) * Matrix3x3::RotationByDegree(gameobjectWorldRotation) * Matrix3x3::Scale(gameobjectWorldScale);

		float length1 = ((Vector2D)(transformMatrix * point1) - (Vector2D)(transformMatrix * localCentroid)).Size();
		float length2 = ((Vector2D)(transformMatrix * point2) - (Vector2D)(transformMatrix * localCentroid)).Size();

		if (length1 < length2)
		{
			return length2;
		}
		else
		{
			return length1;
		}
	}

	float LineCollider::GetFarthestLengthFromWorldPosition()
	{
		Vector2D localCentroid = gameObject->GetTransform()->GetWorldPosition();

		Vector2D gameobjectWorldPosition = gameObject->GetTransform()->GetWorldPosition();
		Vector2D gameobjectWorldScale = gameObject->GetTransform()->GetWorldScale();
		float gameobjectWorldRotation = gameObject->GetTransform()->GetWorldRotation().z;

		Matrix3x3 transformMatrix = Matrix3x3::Translation(gameobjectWorldPosition) * Matrix3x3::RotationByDegree(gameobjectWorldRotation) * Matrix3x3::Scale(gameobjectWorldScale);

		float length1 = ((Vector2D)(transformMatrix * point1) - (Vector2D)(transformMatrix * localCentroid)).Size();
		float length2 = ((Vector2D)(transformMatrix * point2) - (Vector2D)(transformMatrix * localCentroid)).Size();

		if (length1 < length2)
		{
			return length2;
		}
		else
		{
			return length1;
		}
	}

	Vector2D LineCollider::GetSupportPoint(Math::Vector2D directionvector)
	{
		Vector2D gameobjectWorldPosition = gameObject->GetTransform()->GetWorldPosition();
		Vector2D gameobjectWorldScale = gameObject->GetTransform()->GetWorldScale();
		float gameobjectWorldRotation = gameObject->GetTransform()->GetWorldRotation().z;

		Vector2D wpoint1 = Matrix3x3::Translation(gameobjectWorldPosition) * Matrix3x3::RotationByDegree(gameobjectWorldRotation) * Matrix3x3::Scale(gameobjectWorldScale) * point1;
		Vector2D wpoint2 = Matrix3x3::Translation(gameobjectWorldPosition) * Matrix3x3::RotationByDegree(gameobjectWorldRotation) * Matrix3x3::Scale(gameobjectWorldScale) * point2;

		float result1 = DotProduct(wpoint1, directionvector);
		float result2 = DotProduct(wpoint2, directionvector);

		if (result1 > result2)
		{
			return wpoint1;
		}
		else
		{
			return wpoint2;
		}
	}
	Vector2D LineCollider::GetPoint1()
	{
		return point1;
	}
	Vector2D LineCollider::GetPoint2()
	{
		return point2;
	}

	float LineCollider::GetMomentOfInertia(float mass)
	{
		return mass;
	}

	bool LineCollider::IsPointInside(Math::Vector2D point)
	{
		return false;
	}

}