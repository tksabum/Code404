#include "AnimationTest.h"

void AnimationTest::Awake()
{
	animator = GetGameObject()->GetComponent<Animator>();
}

void AnimationTest::Update()
{
	if (Input::GetInstance().PressedKeyNow('Z'))
	{
		animator->Play(L"Animation_Zerg_Hydralisk_Move_Up");
	}

	if (Input::GetInstance().PressedKeyNow('X'))
	{
		animator->Play(L"Animation_Zerg_Hydralisk_Attack_Up");
	}
}
