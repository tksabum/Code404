#include "Matrix3x1.h"

#include "Vector2D.h"
#include "Vector3D.h"

namespace BJEngine
{
	namespace Math
	{
		Matrix3x1::Matrix3x1(float x, float y, float z) : m1_1(x), m2_1(y), m3_1(z)
		{

		}

		Matrix3x1::Matrix3x1(const Vector2D& vector2d) : m1_1(vector2d.x), m2_1(vector2d.y), m3_1(1.0f)
		{

		}

		Matrix3x1::Matrix3x1(const Matrix3x1& matrix3x1) : m1_1(matrix3x1.m1_1), m2_1(matrix3x1.m2_1), m3_1(matrix3x1.m3_1)
		{
			
		}

		Matrix3x1 Matrix3x1::operator=(const Vector2D& right)
		{
			m1_1 = right.x;
			m2_1 = right.y;
			m3_1 = 1.0f;
			return *this;
		}
	}
}