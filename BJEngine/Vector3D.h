#pragma once

#ifdef VECTOR3D_EXPORTS
#define VECTOR3D_API __declspec(dllexport)
#else
#define VECTOR3D_API __declspec(dllimport)
#endif

namespace BJEngine
{
	namespace Math
	{
		class Vector2D;

		class Vector3D
		{
		public:
			float x;
			float y;
			float z;

			VECTOR3D_API Vector3D(float x = 0.0f, float y = 0.0f, float z = 0.0f);
			VECTOR3D_API Vector3D(const Vector3D& vector3d);
			VECTOR3D_API Vector3D(const Vector2D& vector2d);

			// 단위벡터를 리턴
			VECTOR3D_API Vector3D GetUnitVector();
			// 벡터의 크기를 리턴
			VECTOR3D_API float Size();

			VECTOR3D_API Vector3D operator+=(const Vector3D& right);
			VECTOR3D_API Vector3D operator-=(const Vector3D& right);

			VECTOR3D_API Vector3D operator+(const Vector3D& right);
			VECTOR3D_API Vector3D operator-(const Vector3D& right);
			VECTOR3D_API Vector3D operator-();
			VECTOR3D_API Vector3D operator*(const float& right);
			VECTOR3D_API Vector3D operator/(const float& right);
			VECTOR3D_API Vector3D operator=(const Vector3D& right);

			VECTOR3D_API bool operator==(const Vector3D& right) const;
			VECTOR3D_API bool operator!=(const Vector3D& right) const;
			VECTOR3D_API bool operator<(const Vector3D& right) const;
		};
	}
}