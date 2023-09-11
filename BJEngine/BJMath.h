#pragma once

#ifdef MATH_EXPORTS
#define MATH_API __declspec(dllexport)
#else
#define MATH_API __declspec(dllimport)
#endif

namespace BJEngine
{
	namespace Math
	{
		class Vector2D;
		class Vector3D;
		class Matrix3x1;
		class Matrix4x1;
		class Matrix3x3;
		class Matrix4x4;

		extern const double PI;

		MATH_API float DegreeToRadian(float degree);
		MATH_API float RadianToDegree(float radian);

		// �� ������ dot product�� �����Ѵ�.
		MATH_API float DotProduct(Vector2D v0, Vector2D v1);

		// �� ������ cross product�� �����Ѵ�.
		MATH_API Vector3D CrossProduct(Vector3D v0, Vector3D v1);

		/// <summary>
		/// �� ���� �̷�� ȸ�������� �ݽð� ����(��ī��Ʈ ��ǥ�� ����)���� �Ǵ��Ѵ�.
		/// </summary>
		/// <returns>
		/// <para> [�ݽð�]: ��� ���� </para>
		/// <para> [������]: 0���� </para>
		/// <para> [�ð����]: ���� ���� </para>
		/// </returns>
		MATH_API float CCW(Vector2D p0, Vector2D p1, Vector2D p2);


		/// <summary>
		/// ������ ���������� �Ǵ��Ѵ�.
		/// </summary>
		/// <param name="ap1"> ���� a�� �� p1 </param>
		/// <param name="ap2"> ���� a�� �� p2 </param>
		/// <param name="bp1"> ���� b�� �� p1 </param>
		/// <param name="bp2"> ���� b�� �� p2 </param>
		/// <returns> ������ �����ִٸ� true�� ��ȯ�Ѵ�. </returns>
		MATH_API bool DetermineLineOverlap(Vector2D ap1, Vector2D ap2, Vector2D bp1, Vector2D bp2);
	}
}

