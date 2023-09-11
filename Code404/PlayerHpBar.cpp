#include "PlayerHpBar.h"

#include "BJMath.h"

#include "GameObject.h"
#include "Transform.h"
#include "Vector2D.h"
#include "Vector3D.h"

using namespace BJEngine;
using namespace BJEngine::Math;

void PlayerHpBar::SetStatus(float* playermaxhp, float* playercurhp)
{
	playerMaxHp = playermaxhp;
	playerCurHp = playercurhp;
}

void PlayerHpBar::Update()
{
	float hpRatio = *playerCurHp / *playerMaxHp;

	GetGameObject()->GetTransform()->SetLocalScale(Vector2D(hpRatio,1));
}
