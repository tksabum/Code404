#pragma once

#ifdef TRANSFORM_EXPORTS
#define TRANSFORM_API __declspec(dllexport)
#else
#define TRANSFORM_API __declspec(dllimport)
#endif

#include "Component.h"

namespace BJEngine
{
	namespace Math
	{
		class Vector2D;
		class Vector3D;
	}

	class GraphicsSystem;

	class Transform : public Component
	{
	public:
		TRANSFORM_API Transform();
		TRANSFORM_API virtual ~Transform();
		
		TRANSFORM_API Math::Vector3D GetLocalPosition();
		TRANSFORM_API Math::Vector3D GetLocalRotation();
		TRANSFORM_API Math::Vector3D GetLocalScale();

		TRANSFORM_API void SetLocalPosition(Math::Vector3D vector3d);
		TRANSFORM_API void SetLocalRotation(Math::Vector3D vector3d);
		TRANSFORM_API void SetLocalRotation(float rotationZ);
		TRANSFORM_API void SetLocalScale(Math::Vector3D vector3d);


		TRANSFORM_API Math::Vector3D GetWorldPosition();
		TRANSFORM_API Math::Vector3D GetWorldRotation();
		TRANSFORM_API Math::Vector3D GetWorldScale();

		TRANSFORM_API Math::Vector2D GetUIPosition();
		TRANSFORM_API float GetUIRotation();
		TRANSFORM_API Math::Vector2D GetUIScale();

		TRANSFORM_API Math::Vector2D Get2DScreenPosition();
		TRANSFORM_API float Get2DScreenRotation();
		TRANSFORM_API Math::Vector2D Get2DScreenScale();

		//// ������ ���� scale�� ����
		//TRANSFORM_API float TransformToWorldScalar(float r);

		//// targetvector�� worldvector�� ��ȯ
		//TRANSFORM_API Math::Vector2D TransformToWorldVector(Math::Vector2D targetvector);

		//// targetvector�� worldvector�� ��ȯ
		//TRANSFORM_API static Math::Vector2D TransformToWorldVector(Math::Vector2D position, Math::Vector2D scale, float rotation, Math::Vector2D targetvector);

		//// ��ũ�� ��ǥ ��� �Լ��� �ʿ��� ��

	private:
		Math::Vector3D* position;
		Math::Vector3D* rotation;
		Math::Vector3D* scale;

		friend GraphicsSystem;
	};
}