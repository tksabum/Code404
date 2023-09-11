#pragma once

#ifdef MATRIX3X3_EXPORTS
#define MATRIX3X3_API __declspec(dllexport)
#else
#define MATRIX3X3_API __declspec(dllimport)
#endif

namespace BJEngine
{
	namespace Math
	{
		class Vector2D;
		class Matrix3x1;

		class Matrix3x3
		{
		public:
			float m1_1, m1_2, m1_3;
			float m2_1, m2_2, m2_3;
			float m3_1, m3_2, m3_3;

			MATRIX3X3_API Matrix3x3(
				float m1_1, float m1_2, float m1_3,
				float m2_1, float m2_2, float m2_3,
				float m3_1, float m3_2, float m3_3
			);

			// x, y 만큼 이동시키는 변환행렬
			MATRIX3X3_API static Matrix3x3 Translation(float x, float y);
			// MoveVector 만큼 이동시키는 변환행렬
			MATRIX3X3_API static Matrix3x3 Translation(Vector2D movevector);

			// 반시계 방향(데카르트 좌표계 기준)으로 회전하는 변환행렬
			MATRIX3X3_API static Matrix3x3 Rotation(float radian);
			MATRIX3X3_API static Matrix3x3 Rotation(float radian, float x, float y);
			MATRIX3X3_API static Matrix3x3 Rotation(float radian, Vector2D center);
			MATRIX3X3_API static Matrix3x3 RotationByDegree(float degree);
			MATRIX3X3_API static Matrix3x3 RotationByDegree(float degree, float x, float y);
			MATRIX3X3_API static Matrix3x3 RotationByDegree(float degree, Vector2D center);

			// 스케일 변환행렬
			MATRIX3X3_API static Matrix3x3 Scale(Vector2D scale);

			MATRIX3X3_API Matrix3x3 operator*(const Matrix3x3& r);
			MATRIX3X3_API Matrix3x1 operator*(const Matrix3x1& r);
		};
	}
}
