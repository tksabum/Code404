#include "AnimTest.h"

#include "Input.h"
#include "GameObject.h"
#include "Animator.h"

using namespace BJEngine;

void AnimTest::Awake()
{
	anim = GetGameObject()->GetComponent<Animator>();
}

void AnimTest::Update()
{
	if (Input::GetInstance().PressedKeyNow('Q'))
	{
		anim->Play(L"Melee_Enemy_Stand");
	}

	if (Input::GetInstance().PressedKeyNow('W'))
	{
		anim->Play(L"Melee_Enemy_Walk");
	}

	if (Input::GetInstance().PressedKeyNow('E'))
	{
		anim->Play(L"Melee_Enemy_Attack");
	}
}
