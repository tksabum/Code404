#include "BossHandRedOpacity.h"

#include "GameObject.h"
#include "SpriteRenderer.h"

#include "Input.h"

void BossHandRedOpacity::SetHP(float* maxhp, float* curhp)
{
	maxHP = maxhp;
	curHP = curhp;
}

void BossHandRedOpacity::Awake()
{
	renderer = GetGameObject()->GetComponent<SpriteRenderer>();
}

void BossHandRedOpacity::Update()
{
	if (Input::GetInstance().PressedKeyNow('T'))
	{
		*curHP -= 10;
	}

	float hpRatio = *curHP / *maxHP;
	hpRatio = 1.f - hpRatio;
	renderer->SetOpacity(hpRatio);
}
