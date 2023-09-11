#include "Vector2D.h"

#include <cmath>
#include "BJMath.h"
#include "Vector3D.h"
#include "Matrix3x1.h"

namespace BJEngine
{
	namespace Math
	{
		Vector2D::Vector2D(Enum direction)
		{
			if (direction == LEFT)
			{
				x = -1.0f;
				y = 0.0f;
			}
			else if (direction == RIGHT)
			{
				x = 1.0f;
				y = 0.0f;
			}
			else if (direction == UP)
			{
				x = 0.0f;
				y = -1.0f;
			}
			else if (direction == DOWN)
			{
				x = 0.0f;
				y = 1.0f;
			}
			else
			{
				x = 0.0f;
				y = 0.0f;
			}
		}

		Vector2D::Vector2D(float x, float y) : x(x), y(y)
		{

		}

		Vector2D::Vector2D(const Vector2D& vector2d) : x(vector2d.x), y(vector2d.y)
		{

		}

		Vector2D::Vector2D(const Vector3D& vector3d) : x(vector3d.x), y(vector3d.y)
		{

		}

		Vector2D::Vector2D(const Matrix3x1& matrix3x1) : x(matrix3x1.m1_1), y(matrix3x1.m2_1)
		{

		}

		Vector2D Vector2D::GetUnitVector()
		{
			float length = sqrt((x * x) + (y * y));

			if (length == 0.0f)
			{
				return 0.0f;
			}

			return Vector2D(x / sqrt((x * x) + (y * y)), y / sqrt((x * x) + (y * y)));
		}

		float Vector2D::Size()
		{
			return sqrt((x * x) + (y * y));
		}

		Vector2D Vector2D::operator+=(const Vector2D& right)
		{
			this->x += right.x;
			this->y += right.y;

			return *this;
		}

		Vector2D Vector2D::operator-=(const Vector2D& right)
		{
			this->x -= right.x;
			this->y -= right.y;

			return *this;
		}

		Vector2D Vector2D::operator+(const Vector2D& right)
		{
			return Vector2D(this->x + right.x, this->y + right.y);
		}

		Vector2D Vector2D::operator-(const Vector2D& right)
		{
			return Vector2D(this->x - right.x, this->y - right.y);
		}

		Vector2D Vector2D::operator-()
		{
			return Vector2D(-x, -y);
		}

		Vector2D Vector2D::operator*(const float& right)
		{
			return Vector2D(this->x * right, this->y * right);
		}

		Vector2D& Vector2D::operator*=(const float& right)
		{
			this->x *= right;
			this->y *= right;
			return *this;
		}

		Vector2D Vector2D::operator/(const float& right)
		{
			return Vector2D(this->x / right, this->y / right);
		}

		Vector2D& Vector2D::operator/=(const float& right)
		{
			this->x /= right;
			this->y /= right;
			return *this;
		}

		Vector2D Vector2D::operator=(const Vector2D& right)
		{
			x = right.x;
			y = right.y;
			return *this;
		}

		Vector2D Vector2D::operator=(const Matrix3x1& right)
		{
			x = right.m1_1;
			y = right.m2_1;
			return *this;
		}

		bool Vector2D::operator==(const Vector2D& right) const
		{
			return (x == right.x) && (y == right.y);
		}

		bool Vector2D::operator!=(const Vector2D& right) const
		{
			return (x != right.x) || (y != right.y);
		}

		bool Vector2D::operator<(const Vector2D& right) const
		{
			if (y < right.y)
			{
				return true;
			}
			else if (y > right.y)
			{
				return false;
			}
			else
			{
				if (x < right.x)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}

	Math::Vector2D operator*(const float n, const Math::Vector2D& v)
	{
		return Math::Vector2D(n * v.x, n * v.y);
	}
}