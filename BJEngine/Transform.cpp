#include "Transform.h"

#include "CoreSystem.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "GameObject.h"
#include "Scene.h"
#include "Camera.h"
#include "Matrix3x3.h"
#include "Matrix3x1.h"

namespace BJEngine
{
	using namespace Math;

	Transform::Transform() : position(new Vector3D(0.0f, 0.0f, 0.0f)), scale(new Vector3D(1.0f, 1.0f, 1.0f)), rotation(new Vector3D(0.0f, 0.0f, 0.0f))
	{

	}

	Transform::~Transform()
	{
		delete position;
		delete scale;
		delete rotation;
	}

	Vector3D Transform::GetLocalPosition()
	{
		return *position;
	}

	Vector3D Transform::GetLocalRotation()
	{
		return *rotation;
	}

	Vector3D Transform::GetLocalScale()
	{
		return *scale;
	}

	void Transform::SetLocalPosition(Vector3D vector3d)
	{
		*position = vector3d;
	}

	void Transform::SetLocalRotation(Vector3D vector3d)
	{
		*rotation = vector3d;
	}

	void Transform::SetLocalRotation(float rotationZ)
	{
		rotation->z = rotationZ;
	}

	void Transform::SetLocalScale(Vector3D vector3d)
	{
		*scale = vector3d;
	}

	Math::Vector3D Transform::GetWorldPosition()
	{
		GameObject* parentobject = gameObject->GetParent();

		if (parentobject == nullptr)
		{
			return *position;
		}

		Vector3D rootPosition = parentobject->GetTransform()->GetWorldPosition();

		return rootPosition + (*position);
	}

	Math::Vector3D Transform::GetWorldRotation()
	{
		GameObject* parentobject = gameObject->GetParent();

		if (parentobject == nullptr)
		{
			return *rotation;
		}

		Vector3D rootRotation = parentobject->GetTransform()->GetWorldRotation();

		return rootRotation + (*rotation);
	}

	Math::Vector3D Transform::GetWorldScale()
	{
		GameObject* parentobject = gameObject->GetParent();

		if (parentobject == nullptr)
		{
			return *scale;
		}

		Vector3D rootScale = parentobject->GetTransform()->GetWorldScale();

		return Vector3D(rootScale.x * scale->x, rootScale.y * scale->y, rootScale.z * scale->z);
	}

	Vector2D Transform::GetUIPosition()
	{
		// windowsize
		Vector2D currentsize = CoreSystem::GetInstance().GetCurrentWindowSize();
		Vector2D originsize = CoreSystem::GetInstance().GetOriginWindowSize();
		float currentratio = currentsize.x / currentsize.y;
		float originratio = originsize.x / originsize.y;

		Vector2D windowposition(0.0f, 0.0f);
		Vector2D windowscale(1.0f, 1.0f);

		// x 기준
		if (currentratio < originratio)
		{
			windowscale = windowscale * (currentsize.x / originsize.x);
			windowposition.y = (currentsize.y - (currentsize.x / originratio)) / 2.0f;
		}
		else
		{
			windowscale = windowscale * (currentsize.y / originsize.y);
			windowposition.x = (currentsize.x - (originratio * currentsize.y)) / 2.0f;
		}

		Matrix3x3 transformMatrix = Matrix3x3::Translation(windowposition) * Matrix3x3::Scale(windowscale);

		Vector2D worldPosition = GetWorldPosition();

		return transformMatrix * worldPosition;
	}

	float Transform::GetUIRotation()
	{
		return GetWorldRotation().z;
	}

	Vector2D Transform::GetUIScale()
	{
		Vector2D worldScale = GetWorldScale();

		// windowsize
		Vector2D currentsize = CoreSystem::GetInstance().GetCurrentWindowSize();
		Vector2D originsize = CoreSystem::GetInstance().GetOriginWindowSize();
		float currentratio = currentsize.x / currentsize.y;
		float originratio = originsize.x / originsize.y;

		Vector2D windowscale(1.0f, 1.0f);

		// x 기준
		if (currentratio < originratio)
		{
			windowscale = windowscale * (currentsize.x / originsize.x);
		}
		else
		{
			windowscale = windowscale * (currentsize.y / originsize.y);
		}

		return Vector2D(worldScale.x * windowscale.x, worldScale.y * windowscale.y);
	}

	Vector2D Transform::Get2DScreenPosition()
	{
		Camera* maincamera = gameObject->scene->GetMainCamera();
		Transform* cameraTransform = maincamera->GetGameObject()->GetTransform();

		Vector2D cameraPosition = cameraTransform->GetWorldPosition();
		float cameraRotation = cameraTransform->GetWorldRotation().z;
		Vector2D cameraScale = cameraTransform->GetWorldScale();

		Vector2D screenSize = maincamera->GetScreenSize();

		// windowsize
		Vector2D currentsize = CoreSystem::GetInstance().GetCurrentWindowSize();
		Vector2D originsize = CoreSystem::GetInstance().GetOriginWindowSize();
		float currentratio = currentsize.x / currentsize.y;
		float originratio = originsize.x / originsize.y;

		Vector2D windowscale(1.0f, 1.0f);

		// x 기준
		if (currentratio < originratio)
		{
			windowscale = windowscale * (currentsize.x / originsize.x);
		}
		else
		{
			windowscale = windowscale * (currentsize.y / originsize.y);
		}

		Matrix3x3 transformMatrix = Matrix3x3::Translation(screenSize / 2.0f) * Matrix3x3::RotationByDegree(cameraRotation) * Matrix3x3::Scale(cameraScale) * Matrix3x3::Scale(windowscale) * Matrix3x3::Translation(-cameraPosition);

		Vector2D worldPosition = GetWorldPosition();

		return transformMatrix * worldPosition;
	}

	float Transform::Get2DScreenRotation()
	{
		Camera* maincamera = gameObject->scene->GetMainCamera();
		Transform* cameraTransform = maincamera->GetGameObject()->GetTransform();

		float cameraRotation = cameraTransform->GetWorldRotation().z;

		return GetWorldRotation().z + cameraRotation;
	}

	Vector2D Transform::Get2DScreenScale()
	{
		Camera* maincamera = gameObject->scene->GetMainCamera();
		Transform* cameraTransform = maincamera->GetGameObject()->GetTransform();

		Vector2D worldScale = GetWorldScale();
		Vector2D cameraScale = cameraTransform->GetWorldScale();

		// windowsize
		Vector2D currentsize = CoreSystem::GetInstance().GetCurrentWindowSize();
		Vector2D originsize = CoreSystem::GetInstance().GetOriginWindowSize();
		float currentratio = currentsize.x / currentsize.y;
		float originratio = originsize.x / originsize.y;

		Vector2D windowscale(1.0f, 1.0f);

		// x 기준
		if (currentratio < originratio)
		{
			windowscale = windowscale * (currentsize.x / originsize.x);
		}
		else
		{
			windowscale = windowscale * (currentsize.y / originsize.y);
		}

		return Vector2D(worldScale.x * cameraScale.x * windowscale.x, worldScale.y * cameraScale.y * windowscale.y);
	}
}