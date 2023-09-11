#include "TimeLaser.h"

#include "GameObject.h"
#include "TimeController.h"

#include "SoundManager.h"

#include "Scene.h"

void TimeLaser::Awake()
{
	effectActiveTime = ablePeriod / 2.f;
	timeLaser = GetGameObject()->GetChild(L"timeLaser");
	leftHoleEffect = GetGameObject()->GetScene()->GetGameObject(L"leftHoleEffect");
	rightHoleEffect = GetGameObject()->GetScene()->GetGameObject(L"rightHoleEffect");
}

void TimeLaser::Update()
{
	float deltaTime = GetGameObject()->GetTimeController()->GetDeltaTime();


	if (!isAble)
	{
		totalAblePeriod += deltaTime;
		if (totalAblePeriod >= ablePeriod)
		{
			isAble = true;

			timeLaser->SetActive(true);
			leftHoleEffect->SetActive(false);
			rightHoleEffect->SetActive(false);
		}
	}
	

	if (!isAble)
	{
		if (!isEffect)
		{
			totalEffectActiveTime += deltaTime;
			if (totalEffectActiveTime >= effectActiveTime)
			{
				SoundManager::GetInstance().Play(SOUND_EFFECT_BOSSLAZER);
				leftHoleEffect->SetActive(true);
				rightHoleEffect->SetActive(true);
				isEffect = true;
			}
		}
	}

	if (isAble)
	{
		totalDisablePeriod += deltaTime;
		if (totalDisablePeriod >= disablePeriod)
		{
			totalAblePeriod = 0.f;
			totalDisablePeriod = 0.f;
			totalEffectActiveTime = 0.f;
			timeLaser->SetActive(false);
			isAble = false;
			isEffect = false;
		}
	}


	
}
