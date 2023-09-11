#include "BJMath.h"

#include "Vector2D.h"
#include "Vector3D.h"

#include <cmath>

namespace BJEngine
{
	namespace Math
	{
		const double PI = std::acos(-1.0);

		float DegreeToRadian(float degree)
		{
			return (degree * PI) / 180.0f;
		}

		float RadianToDegree(float radian)
		{
			return (radian * 180.0f) / PI;
		}

		float DotProduct(Vector2D v0, Vector2D v1)
		{
			return (v0.x * v1.x) + (v0.y * v1.y);
		}

		Vector3D CrossProduct(Vector3D v0, Vector3D v1)
		{
			return Vector3D((v0.y * v1.z - v0.z * v1.y), (v0.z * v1.x - v0.x * v1.z), (v0.x * v1.y - v0.y * v1.x));
		}

		float CCW(Vector2D p0, Vector2D p1, Vector2D p2)
		{
			return CrossProduct((p1 - p0), (p2 - p0)).z;
		}

		bool DetermineLineOverlap(Vector2D ap1, Vector2D ap2, Vector2D bp1, Vector2D bp2)
		{
			return (CCW(ap1, ap2, bp1) * CCW(ap1, ap2, bp2) <= 0.0f) && (CCW(bp1, bp2, ap1) * CCW(bp1, bp2, ap2) <= 0.0f);
		}
	}
}