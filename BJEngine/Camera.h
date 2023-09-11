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

		// 3d에서 화면상의 좌표를 월드좌표의 벡터로 바꿔주는 함수 필요한 경우 추가

	private:


	};
}