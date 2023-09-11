#include "ConvexpolygonCollider.h"

#include "GameObject.h"
#include "Transform.h"
#include "BJMath.h"
#include "Matrix3x3.h"
#include "Matrix3x1.h"

using namespace std;

namespace BJEngine
{
	using namespace Math;

	ConvexpolygonCollider::ConvexpolygonCollider() : points(vector<Vector2D>())
	{

	}

	ConvexpolygonCollider::~ConvexpolygonCollider()
	{

	}

	void ConvexpolygonCollider::SetPolygon(const std::vector<Math::Vector2D>& vertexList)
	{
		points = vertexList;
	}

	void ConvexpolygonCollider::SetRegularPolygon(int vertexcount, float sidesize)
	{
		// 초기화
		points.clear();

		// 원점부터 시작하여 정다각형 그리기
		Vector2D p(0.0f, 0.0f);
		Vector2D direction(-1.0f, 0.0f);
		Matrix3x3 rotate = Matrix3x3::RotationByDegree(360.0f / vertexcount);

		for (int i = 0; i < vertexcount; i++)
		{
			points.push_back(p);
			p = p + (direction * sidesize);
			direction = rotate * direction;
		}

		// 무게중심 찾기
		Vector2D centroid(0.0f, 0.0f);

		for (int i = 0; i < points.size(); i++)
		{
			centroid = centroid + points[i];
		}

		centroid = centroid / points.size();

		// 무게중심이 원점이 되도록 이동
		for (int i = 0; i < points.size(); i++)
		{
			points[i] = points[i] - centroid;
		}
	}

	vector<Vector2D> ConvexpolygonCollider::GetPoints()
	{
		return points;
	}

	Vector2D ConvexpolygonCollider::GetCentroid()
	{
		Vector2D localCentroid(0.0f, 0.0f);

		for (int i = 0; i < points.size(); i++)
		{
			localCentroid = localCentroid + points[i];
		}

		if (points.size() != 0)
		{
			localCentroid = localCentroid / points.size();
		}

		Vector2D gameobjectWorldPosition = gameObject->GetTransform()->GetWorldPosition();
		Vector2D gameobjectWorldScale = gameObject->GetTransform()->GetWorldScale();
		float gameobjectWorldRotation = gameObject->GetTransform()->GetWorldRotation().z;

		Matrix3x3 transformMatrix = Matrix3x3::Translation(gameobjectWorldPosition) * Matrix3x3::RotationByDegree(gameobjectWorldRotation) * Matrix3x3::Scale(gameobjectWorldScale);

		return transformMatrix * localCentroid;
	}

	float ConvexpolygonCollider::GetFarthestLength()
	{
		Vector2D gameobjectWorldScale = gameObject->GetTransform()->GetWorldScale();

		Vector2D localCentroid(0.0f, 0.0f);

		for (int i = 0; i < points.size(); i++)
		{
			localCentroid = localCentroid + points[i];
		}

		if (points.size() != 0)
		{
			localCentroid = localCentroid / points.size();
		}

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

	float ConvexpolygonCollider::GetFarthestLengthFromWorldPosition()
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

	float ConvexpolygonCollider::GetMomentOfInertia(float mass)
	{
		Vector2D localCentroid(0.0f, 0.0f);

		for (int i = 0; i < points.size(); i++)
		{
			localCentroid = localCentroid + points[i];
		}

		if (points.size() != 0)
		{
			localCentroid = localCentroid / points.size();
		}

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

		//return mass;
	}

	bool ConvexpolygonCollider::IsPointInside(Vector2D point)
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

	Vector2D ConvexpolygonCollider::GetSupportPoint(Vector2D directionvector)
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