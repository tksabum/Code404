#include "PlayerMpBar.h"

#include "BJMath.h"

#include "GameObject.h"
#include "Transform.h"
#include "Vector2D.h"
#include "Vector3D.h"

using namespace BJEngine;
using namespace BJEngine::Math;

void PlayerMpBar::SetStatus(float* playermaxmp, float* playercurmp)
{
	playerMaxMp = playermaxmp;
	playerCurMp = playercurmp;
}

void PlayerMpBar::Update()
{
	float mpRatio = *playerCurMp / *playerMaxMp;

	GetGameObject()->GetTransform()->SetLocalScale(Vector2D(mpRatio, 1));
}