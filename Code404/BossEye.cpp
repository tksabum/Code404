#include "BossEye.h"

#include "BJMath.h"
#include "Vector2D.h"
#include "Vector3D.h"

#include "GameObject.h"
#include "Transform.h"

using namespace BJEngine;
using namespace BJEngine::Math;

void BossEye::SetTarget(BJEngine::GameObject* targetobject)
{
	target = targetobject;
}

void BossEye::Update()
{
	Vector3D dir = target->GetTransform()->GetLocalPosition() - GetGameObject()->GetTransform()->GetWorldPosition();
	dir = dir.GetUnitVector();
	dir = dir * maxDistance;

	GetGameObject()->GetTransform()->SetLocalPosition(dir);
}
