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

		// 두 벡터의 dot product를 리턴한다.
		MATH_API float DotProduct(Vector2D v0, Vector2D v1);

		// 두 벡터의 cross product를 리턴한다.
		MATH_API Vector3D CrossProduct(Vector3D v0, Vector3D v1);

		/// <summary>
		/// 세 점이 이루는 회전방향이 반시계 방향(데카르트 좌표계 기준)인지 판단한다.
		/// </summary>
		/// <returns>
		/// <para> [반시계]: 양수 리턴 </para>
		/// <para> [일직선]: 0리턴 </para>
		/// <para> [시계방향]: 음수 리턴 </para>
		/// </returns>
		MATH_API float CCW(Vector2D p0, Vector2D p1, Vector2D p2);


		/// <summary>
		/// 선분이 겹쳐졌는지 판단한다.
		/// </summary>
		/// <param name="ap1"> 선분 a의 점 p1 </param>
		/// <param name="ap2"> 선분 a의 점 p2 </param>
		/// <param name="bp1"> 선분 b의 점 p1 </param>
		/// <param name="bp2"> 선분 b의 점 p2 </param>
		/// <returns> 선분이 겹쳐있다면 true를 반환한다. </returns>
		MATH_API bool DetermineLineOverlap(Vector2D ap1, Vector2D ap2, Vector2D bp1, Vector2D bp2);
	}
}

