#pragma once

#ifdef MATRIX3X1_EXPORTS
#define MATRIX3X1_API __declspec(dllexport)
#else
#define MATRIX3X1_API __declspec(dllimport)
#endif

namespace BJEngine
{
	namespace Math
	{
		class Vector2D;

		class Matrix3x1
		{
		public:
			float m1_1;
			float m2_1;
			float m3_1;
			MATRIX3X1_API Matrix3x1(float x, float y, float z = 1.0f);
			MATRIX3X1_API Matrix3x1(const Vector2D& vector2d);
			MATRIX3X1_API Matrix3x1(const Matrix3x1& matrix3x1);

			MATRIX3X1_API Matrix3x1 operator=(const Vector2D& right);
		};
	}
}

