#include "Camera.h"

#include "GraphicsSystem.h"
#include "Matrix3x3.h"
#include "Matrix3x1.h"
#include "GameObject.h"
#include "Transform.h"

namespace BJEngine
{
	using namespace Math;

	Camera::Camera()
	{

	}

	Camera::~Camera()
	{

	}

	Vector2D Camera::GetScreenSize()
	{
		return GraphicsSystem::GetInstance().GetWindowSize();
	}

	Math::Vector2D Camera::ConvertTo2DWorldPosition(Math::Vector2D screenPosition)
	{
		Vector2D cameraPosition = gameObject->GetTransform()->GetWorldPosition();
		float cameraRotation = gameObject->GetTransform()->GetWorldRotation().z;
		Vector2D cameraScale = gameObject->GetTransform()->GetWorldScale();
		Vector2D inverseCameraScale = Vector2D(1.0f / cameraScale.x, 1.0f / cameraScale.y);
		
		Matrix3x3 transformMatrix = Matrix3x3::Translation(cameraPosition) * Matrix3x3::RotationByDegree(-cameraRotation) * Matrix3x3::Scale(inverseCameraScale) * Matrix3x3::Translation(-GetScreenSize() / 2.0f);

		return transformMatrix * screenPosition;
	}
}