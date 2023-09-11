#include "GamePause.h"

void GamePause::OnEnable()
{
	GetGameObject()->GetScene()->GetTimeController()->SetTimeScale(0.0f);
	Input::GetInstance().SetKeyActive(false);
}

void GamePause::OnDisable()
{
	GetGameObject()->GetScene()->GetTimeController()->SetTimeScale(1.0f);
	Input::GetInstance().SetKeyActive(true);
}
