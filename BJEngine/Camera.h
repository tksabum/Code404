#pragma once

#ifdef CAMERA_EXPORTS
#define CAMERA_API __declspec(dllexport)
#else
#define CAMERA_API __declspec(dllimport)
#endif

#include "Component.h"

#include "Vector2D.h"
#include "Vector3D.h"

namespace BJEngine
{
	namespace Math
	{
		class Vector2D;
	}

	class Camera : public Component
	{
	public:
		CAMERA_API Camera();
		CAMERA_API ~Camera();

		CAMERA_API Math::Vector2D GetScreenSize();

		CAMERA_API Math::Vector2D ConvertTo2DWorldPosition(Math::Vector2D screenPosition);

		// 3d���� ȭ����� ��ǥ�� ������ǥ�� ���ͷ� �ٲ��ִ� �Լ� �ʿ��� ��� �߰�

	private:


	};
}