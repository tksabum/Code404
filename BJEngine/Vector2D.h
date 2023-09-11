#pragma once

#ifdef VECTOR2D_EXPORTS
#define VECTOR2D_API __declspec(dllexport)
#else
#define VECTOR2D_API __declspec(dllimport)
#endif

namespace BJEngine
{
	namespace Math
	{
		class Vector3D;
		class Matrix3x1;

		class Vector2D
		{
		public:
			enum Enum
			{
				LEFT,
				RIGHT,
				UP,
				DOWN
			};

			float x;
			float y;

			VECTOR2D_API Vector2D(Enum direction);
			VECTOR2D_API Vector2D(float x = 0.0f, float y = 0.0f);
			VECTOR2D_API Vector2D(const Vector2D& vector2d);
			VECTOR2D_API Vector2D(const Vector3D& vector3d);
			VECTOR2D_API Vector2D(const Matrix3x1& matrix3x1);

			// 단위벡터를 리턴
			VECTOR2D_API Vector2D GetUnitVector();
			// 벡터의 크기를 리턴
			VECTOR2D_API float Size();

			VECTOR2D_API Vector2D operator+=(const Vector2D& right);
			VECTOR2D_API Vector2D operator-=(const Vector2D& right);

			VECTOR2D_API Vector2D operator+(const Vector2D& right);
			VECTOR2D_API Vector2D operator-(const Vector2D& right);
			VECTOR2D_API Vector2D operator-();
			
			VECTOR2D_API Vector2D operator*(const float& right);
			VECTOR2D_API Vector2D& operator*=(const float& right);
			VECTOR2D_API Vector2D operator/(const float& right);
			VECTOR2D_API Vector2D& operator/=(const float& right);
			
			VECTOR2D_API Vector2D operator=(const Vector2D& right);
			VECTOR2D_API Vector2D operator=(const Matrix3x1& right);

			VECTOR2D_API bool operator==(const Vector2D& right) const;
			VECTOR2D_API bool operator!=(const Vector2D& right) const;
			VECTOR2D_API bool operator<(const Vector2D& right) const;

			friend Math::Vector2D operator*(const float n, const Math::Vector2D& v);
		};
	}
}
