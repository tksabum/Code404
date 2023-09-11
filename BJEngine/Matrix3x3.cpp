#include "Matrix3x3.h"

#include <cmath>

#include "BJMath.h"
#include "Vector2D.h"
#include "Matrix3x1.h"

namespace BJEngine
{
	namespace Math
	{
		Matrix3x3::Matrix3x3(float m1_1, float m1_2, float m1_3, float m2_1, float m2_2, float m2_3, float m3_1, float m3_2, float m3_3) :
			m1_1(m1_1), m1_2(m1_2), m1_3(m1_3),
			m2_1(m2_1), m2_2(m2_2), m2_3(m2_3),
			m3_1(m3_1), m3_2(m3_2), m3_3(m3_3)
		{

		}

		Matrix3x3 Matrix3x3::Translation(float x, float y)
		{
			return Matrix3x3
			(
				1.0f, 0.0f, x,
				0.0f, 1.0f, y,
				0.0f, 0.0f, 1.0f
			);
		}

		Matrix3x3 Matrix3x3::Translation(Vector2D movevector)
		{
			return Matrix3x3
			(
				1.0f, 0.0f, movevector.x,
				0.0f, 1.0f, movevector.y,
				0.0f, 0.0f, 1.0f
			);
		}
		Matrix3x3 Matrix3x3::Rotation(float radian)
		{
			return Matrix3x3
			(
				cos(radian),	-sin(radian),	0.0f,
				sin(radian),	cos(radian),	0.0f,
				0.0f,			0.0f,			1.0f
			);
		}
		Matrix3x3 Matrix3x3::Rotation(float radian, float x, float y)
		{
			return Translation(x, y) * Rotation(radian) * Translation(-x, -y);
		}
		Matrix3x3 Matrix3x3::Rotation(float radian, Vector2D center)
		{
			return Rotation(radian, center.x, center.y);
		}

		Matrix3x3 Matrix3x3::RotationByDegree(float degree)
		{
			float radian = (degree / 180.0f) * PI;
			return Rotation(radian);
		}
		Matrix3x3 Matrix3x3::RotationByDegree(float degree, float x, float y)
		{
			float radian = (degree / 180.0f) * PI;
			return Rotation(radian, x, y);
		}
		Matrix3x3 Matrix3x3::RotationByDegree(float degree, Vector2D center)
		{
			float radian = (degree / 180.0f) * PI;
			return Rotation(radian, center.x, center.y);
		}

		Matrix3x3 Matrix3x3::Scale(Vector2D scale)
		{
			return Matrix3x3
			(
				scale.x * 1.0f, 0.0f,			0.0f,
				0.0f,			scale.y * 1.0f,	0.0f,
				0.0f,			0.0f,			1.0f
			);
		}

		Matrix3x3 Matrix3x3::operator*(const Matrix3x3& r)
		{
			return Matrix3x3
			(
				(m1_1 * r.m1_1) + (m1_2 * r.m2_1) + (m1_3 * r.m3_1), (m1_1 * r.m1_2) + (m1_2 * r.m2_2) + (m1_3 * r.m3_2), (m1_1 * r.m1_3) + (m1_2 * r.m2_3) + (m1_3 * r.m3_3),
				(m2_1 * r.m1_1) + (m2_2 * r.m2_1) + (m2_3 * r.m3_1), (m2_1 * r.m1_2) + (m2_2 * r.m2_2) + (m2_3 * r.m3_2), (m2_1 * r.m1_3) + (m2_2 * r.m2_3) + (m2_3 * r.m3_3),
				(m3_1 * r.m1_1) + (m3_2 * r.m2_1) + (m3_3 * r.m3_1), (m3_1 * r.m1_2) + (m3_2 * r.m2_2) + (m3_3 * r.m3_2), (m3_1 * r.m1_3) + (m3_2 * r.m2_3) + (m3_3 * r.m3_3)
			);
		}

		Matrix3x1 Matrix3x3::operator*(const Matrix3x1& r)
		{
			return Matrix3x1
			(
				(m1_1 * r.m1_1) + (m1_2 * r.m2_1) + (m1_3 * r.m3_1),
				(m2_1 * r.m1_1) + (m2_2 * r.m2_1) + (m2_3 * r.m3_1),
				(m3_1 * r.m1_1) + (m3_2 * r.m2_1) + (m3_3 * r.m3_1)
			);
		}
	}
}