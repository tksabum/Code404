#include "ParryingSystem.h"

#include "SoundManager.h"

const float ParryingSystem::ORIGINAL_DELTA = 1.0f;
const float ParryingSystem::SLOW_RATE = 0.1f;
const float ParryingSystem::SLOW_TIME = 0.5f;

void ParryingSystem::Awake()
{
	timecontroller = GetGameObject()->GetScene()->GetTimeController();
}

void ParryingSystem::Update()
{
	elapsedTime += timecontroller->GetDeltaTime();
	if ((timecontroller->GetTimeScale() != 0.0f) && (timecontroller->GetTimeScale() != ORIGINAL_DELTA) && (elapsedTime >= SLOW_TIME * SLOW_RATE))
	{
		timecontroller->SetTimeScale(ORIGINAL_DELTA);
	}
}

void ParryingSystem::Parrying()
{
	elapsedTime = 0.0f;
	timecontroller->SetTimeScale(SLOW_RATE);

	SoundManager::GetInstance().Play(SOUND_EFFECT_PARRYING);
}