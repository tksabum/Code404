#include "ItemMove.h"

#include "GameObject.h"
#include "Transform.h"
#include "Vector3D.h"
#include "BJMath.h"
#include "TimeController.h"

#include <cmath>

using namespace BJEngine;
using namespace BJEngine::Math;

void ItemMove::Awake()
{
	originPosY = GetGameObject()->GetTransform()->GetLocalPosition().y;
}

void ItemMove::Update()
{
	float deltaTime = GetGameObject()->GetTimeController()->GetDeltaTime();
	Vector3D pos = GetGameObject()->GetTransform()->GetLocalPosition();

	angle += moveSpeed * deltaTime;

	if (angle >= 360.f)
	{
		angle = 0.f;
	}


	pos.y = originPosY+ verticalOffset * sin(angle);
	GetGameObject()->GetTransform()->SetLocalPosition(pos);
}
