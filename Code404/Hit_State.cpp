#include "Hit_State.h"

#include "AI.h"
#include "GameObject.h"
#include "Animator.h"

#include "Enemy.h"

#include "TimeController.h"

using namespace BJEngine;

bool Hit_State::GetAttacked()
{
	return isAttacked;
}

void Hit_State::Update()
{
	totalHitTime += GetAI()->GetGameObject()->GetTimeController()->GetDeltaTime();
	if (totalHitTime >= hitTime)
	{
		GetAI()->ChangeState(AI_STATE::IDLE);
	}
}

void Hit_State::Enter()
{
	totalHitTime = 0.f;
	isAttacked = true;

	ENEMY_TYPE type = GetAI()->GetEnemyType();

	switch (type)
	{
		case ENEMY_TYPE::MeleeA_Enemy:
		{
			GetAnimator()->Play(L"MeleeA_Enemy_Hit");
		}
			break;
		case ENEMY_TYPE::MeleeB_Enemy:
		{
			GetAnimator()->Play(L"MeleeB_Enemy_Hit");
		}
			break;
		case ENEMY_TYPE::RangeA_Enemy:
		{
			GetAnimator()->Play(L"RangeA_Enemy_Hit");
		}
		break;
		case ENEMY_TYPE::RangeB_Enemy:
		{
			GetAnimator()->Play(L"RangeB_Enemy_Hit");
		}
		break;
	}
}

void Hit_State::Exit()
{
	totalHitTime = 0.f;
	isAttacked = false;
	enemyComponent->SetIsAttacked(false);
}

Hit_State::Hit_State(AI* ai)
	: State(ai)
{

}

Hit_State::~Hit_State()
{

}
