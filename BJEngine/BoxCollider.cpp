#include "BoxCollider.h"

#include "GameObject.h"
#include "Transform.h"
#include "BJMath.h"
#include "Matrix3x3.h"
#include "Matrix3x1.h"

namespace BJEngine
{
	using namespace Math;

	BoxCollider::BoxCollider() : points()
	{

	}

	BoxCollider::~BoxCollider()
	{

	}

	void BoxCollider::SetRectangle(float width, float hight)
	{
		points.push_back({ -(width / 2), -(hight / 2) });
		points.push_back({ (width / 2), -(hight / 2) });
		points.push_back({ width / 2, hight / 2 });
		points.push_back({ -(width / 2), hight / 2 });
	}

	std::vector<Vector2D> BoxCollider::GetPoints()
	{
		return points;
	}

	Vector2D BoxCollider::GetCentroid()
	{
		Vector2D localCentroid(0.0f, 0.0f);

		Vector2D gameobjectWorldPosition = gameObject->GetTransform()->GetWorldPosition();
		Vector2D gameobjectWorldScale = gameObject->GetTransform()->GetWorldScale();
		float gameobjectWorldRotation = gameObject->GetTransform()->GetWorldRotation().z;

		Matrix3x3 transformMatrix = Matrix3x3::Translation(gameobjectWorldPosition) * Matrix3x3::RotationByDegree(gameobjectWorldRotation) * Matrix3x3::Scale(gameobjectWorldScale);

		return transformMatrix * localCentroid;
	}

	float BoxCollider::GetFarthestLength()
	{
		Vector2D gameobjectWorldScale = gameObject->GetTransform()->GetWorldScale();

		Vector2D localCentroid(0.0f, 0.0f);

		Matrix3x3 transformMatrix = Matrix3x3::Scale(gameobjectWorldScale);

		float maxvalue = -(INFINITY);

		for (int i = 0; i < points.size(); i++)
		{
			Vector2D scaledPoint = transformMatrix * points[i];
			Vector2D scaledCentroid = transformMatrix * localCentroid;

			if (maxvalue < (scaledPoint - scaledCentroid).Size())
			{
				maxvalue = (scaledPoint - scaledCentroid).Size();
			}
		}

		return maxvalue;
	}

	float BoxCollider::GetFarthestLengthFromWorldPosition()
	{
		Vector2D gameobjectWorldScale = gameObject->GetTransform()->GetWorldScale();

		Vector2D localCentroid(0.0f, 0.0f);

		Matrix3x3 transformMatrix = Matrix3x3::Scale(gameobjectWorldScale);

		float maxvalue = -(INFINITY);

		for (int i = 0; i < points.size(); i++)
		{
			Vector2D scaledPoint = transformMatrix * points[i];
			Vector2D scaledCentroid = transformMatrix * localCentroid;

			if (maxvalue < (scaledPoint - scaledCentroid).Size())
			{
				maxvalue = (scaledPoint - scaledCentroid).Size();
			}
		}

		return maxvalue;
	}

	float BoxCollider::GetMomentOfInertia(float mass)
	{
		Vector2D localCentroid(0.0f, 0.0f);

		float inertia = 0.0f;
		float area = 0.0f;

		for (int i = 0; i < points.size(); i++) {
			Vector2D v1 = (points[i] - localCentroid) / 30.0f;
			Vector2D v2 = (points[(i + 1) % points.size()] - localCentroid) / 30.0f;
			float cross = CrossProduct(v1, v2).z;
			area += cross;
			inertia += cross * (DotProduct(v1, v1) + DotProduct(v1, v2) + DotProduct(v2, v2));
		}

		inertia *= mass / 6.0f * area;

		return inertia;
	}

	bool BoxCollider::IsPointInside(Vector2D point)
	{
		Vector2D gameobjectWorldPosition = gameObject->GetTransform()->GetWorldPosition();
		Vector2D gameobjectWorldScale = gameObject->GetTransform()->GetWorldScale();
		float gameobjectWorldRotation = gameObject->GetTransform()->GetWorldRotation().z;

		Matrix3x3 transformMatrix = Matrix3x3::Translation(gameobjectWorldPosition) * Matrix3x3::RotationByDegree(gameobjectWorldRotation) * Matrix3x3::Scale(gameobjectWorldScale);

		bool cwresult = true;
		bool ccwresult = true;

		for (int i = 0; i < points.size(); i++)
		{
			Vector2D wpoint1 = transformMatrix * points[i];
			Vector2D wpoint2 = transformMatrix * points[(i + 1) % points.size()];

			float ccw = CCW(wpoint1, wpoint2, point);

			if (ccw < 0.0f)
			{
				ccwresult = false;
			}

			if (ccw > 0.0f)
			{
				cwresult = false;
			}
		}

		return cwresult || ccwresult;
	}

	Vector2D BoxCollider::GetSupportPoint(Vector2D directionvector)
	{
		Vector2D gameobjectWorldPosition = gameObject->GetTransform()->GetWorldPosition();
		Vector2D gameobjectWorldScale = gameObject->GetTransform()->GetWorldScale();
		float gameobjectWorldRotation = gameObject->GetTransform()->GetWorldRotation().z;

		Matrix3x3 transformMatrix = Matrix3x3::Translation(gameobjectWorldPosition) * Matrix3x3::RotationByDegree(gameobjectWorldRotation) * Matrix3x3::Scale(gameobjectWorldScale);

		float maxResult = -INFINITY;
		Vector2D supportPoint;

		for (int i = 0; i < points.size(); i++)
		{
			Vector2D wpoint = transformMatrix * points[i];
			float dotResult = DotProduct(wpoint, directionvector);
			if (maxResult < dotResult)
			{
				maxResult = dotResult;
				supportPoint = wpoint;
			}
		}

		return supportPoint;
	}
}