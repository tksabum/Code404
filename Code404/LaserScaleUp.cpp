#include "LaserScaleUp.h"

#include "GameObject.h"
#include "Transform.h"
#include "Vector3D.h"

#include "TimeController.h"

#include "BJMath.h"

using namespace BJEngine;
using namespace BJEngine::Math;

void LaserScaleUp::Update()
{
	Vector3D scale = GetGameObject()->GetTransform()->GetLocalScale();
	float deltaTime = GetGameObject()->GetTimeController()->GetDeltaTime();
	if (isScaleUp)
	{
		if (scale.x >= 1.f && scale.y >= 1.f && scale.z >= 1.f)
		{
			scale = Vector3D(1, 1, 1);
			isScaleUp = false;
		}
		else
		{
			scale.x += scaleUpSpeed * deltaTime;
			scale.y += scaleUpSpeed * deltaTime;
		}
		GetGameObject()->GetTransform()->SetLocalScale(scale);
	}
}

void LaserScaleUp::OnDisable()
{
	GetGameObject()->GetTransform()->SetLocalScale(Vector3D(0, 0, 1));
}

void LaserScaleUp::OnEnable()
{
	isScaleUp = true;
	GetGameObject()->GetTransform()->SetLocalRotation(45);
}
