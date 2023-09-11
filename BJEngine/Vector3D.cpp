#include "Vector3D.h"

#include <cmath>
#include "Vector2D.h"

namespace BJEngine
{
	namespace Math
	{
		Vector3D::Vector3D(float x, float y, float z) : x(x), y(y), z(z)
		{

		}

		Vector3D::Vector3D(const Vector3D& vector3d) : x(vector3d.x), y(vector3d.y), z(vector3d.z)
		{

		}

		Vector3D::Vector3D(const Vector2D& vector2d) : x(vector2d.x), y(vector2d.y), z(0.0f)
		{

		}

		Vector3D Vector3D::GetUnitVector()
		{
			float length = sqrt((x * x) + (y * y) + (z * z));

			if (length == 0.0f)
			{
				return 0.0f;
			}

			return Vector3D(x / length, y / length, z / length);
		}

		float Vector3D::Size()
		{
			return sqrt((x * x) + (y * y) + (z * z));
		}

		Vector3D Vector3D::operator+=(const Vector3D& right)
		{
			this->x += right.x;
			this->y += right.y;
			this->z += right.z;

			return *this;
		}

		Vector3D Vector3D::operator-=(const Vector3D& right)
		{
			this->x -= right.x;
			this->y -= right.y;
			this->z -= right.z;

			return *this;
		}

		Vector3D Vector3D::operator+(const Vector3D& right)
		{
			return Vector3D(x + right.x, y + right.y, z + right.z);
		}

		Vector3D Vector3D::operator-(const Vector3D& right)
		{
			return Vector3D(x - right.x, y - right.y, z - right.z);
		}

		Vector3D Vector3D::operator-()
		{
			return Vector3D(-x, -y, -z);
		}

		Vector3D Vector3D::operator*(const float& right)
		{
			return Vector3D(x * right, y * right, z * right);
		}

		Vector3D Vector3D::operator/(const float& right)
		{
			return Vector3D(x / right, y / right, z / right);
		}

		Vector3D Vector3D::operator=(const Vector3D& right)
		{
			x = right.x;
			y = right.y;
			z = right.z;
			return *this;
		}

		bool Vector3D::operator==(const Vector3D& right) const
		{
			return (x == right.x) && (y == right.y) && (z == right.z);
		}

		bool Vector3D::operator!=(const Vector3D& right) const
		{
			return (x != right.x) || (y != right.y) || (z != right.z);
		}

		bool Vector3D::operator<(const Vector3D& right) const
		{
			if (z < right.z)
			{
				return true;
			}
			else if (z > right.z)
			{
				return false;
			}
			else
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
	}
}