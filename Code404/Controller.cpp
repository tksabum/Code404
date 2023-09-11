#include "Controller.h"

#include "CameraShake.h"

void Controller::Awake()
{
	moveSpeed = 20.0f;
	scaleSpeed = 1.2f;
}

void Controller::Update()
{
	Transform* transform = GetGameObject()->GetTransform();

	Vector2D positionDelta(0.0f, 0.0f);

	if (Input::GetInstance().PressedKey('J'))
	{
		positionDelta += Vector2D(-3.0f, 0.0f);
	}
	if (Input::GetInstance().PressedKey('L'))
	{
		positionDelta += Vector2D(3.0f, 0.0f);
	}
	if (Input::GetInstance().PressedKey('I'))
	{
		positionDelta += Vector2D(0.0f, -3.0f);
	}
	if (Input::GetInstance().PressedKey('K'))
	{
		positionDelta += Vector2D(0.0f, 3.0f);
	}

	transform->SetLocalPosition(transform->GetLocalPosition() + positionDelta);

	if (Input::GetInstance().GetWheelDelta() != 0)
	{
		transform->SetLocalScale(transform->GetLocalScale() * pow(scaleSpeed, Input::GetInstance().GetWheelDelta() / 120.0f));
	}
}
