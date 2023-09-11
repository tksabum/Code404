#include "Idle_State.h"

#include <Math.h>

#include "CoreSystem.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TimeController.h"

#include "Trace_State.h"
#include "Patrol_State.h"

#include "AI.h"

#include "BJMath.h"
#include "Vector3D.h"
#include "GameObject.h"
#include "Transform.h"

#include "Animator.h"

using namespace BJEngine;
using namespace BJEngine::Math;

Idle_State::Idle_State(AI* ai, float idletime)
	: State(ai)
	, idleTime(idletime)
{

}

Idle_State::~Idle_State()
{

}

void Idle_State::Update()
{
	//  IDLE,
	//	PATROL,
	//	TRACETARGET,
	//	NORMALATTACK,
	//	DIE,

	// 변경 가능성있는 상태
	// 1. Die
	// 2. Patrol
	// 3. Trace
	// 4. AttackS

	State::Update();

	AI* ai = GetAI();
	Vector3D ownerpos = ai->GetGameObject()->GetTransform()->GetLocalPosition();
	std::map<AI_STATE, State*>& mapTemp = ai->GetMapState();

	totalIdleTime += CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetDeltaTime();

	// 패트롤 기능이 있다면
	if (mapTemp[AI_STATE::PATROL])
	{
		if (totalIdleTime >= idleTime)
		{
			totalIdleTime = 0.f;
			ai->ChangeState(AI_STATE::PATROL);
		}
	}
}

void Idle_State::Enter()
{
	totalIdleTime = 0.f;
	ENEMY_TYPE type = GetAI()->GetEnemyType();
	switch (type)
	{
		case ENEMY_TYPE::MeleeA_Enemy:
		{
			GetAnimator()->Play(L"MeleeA_Enemy_Stand");
		}
			break;
		case ENEMY_TYPE::MeleeB_Enemy:
		{
			GetAnimator()->Play(L"MeleeB_Enemy_Stand");
		}
		break;
		case ENEMY_TYPE::RangeA_Enemy:
		{
			GetAnimator()->Play(L"RangeA_Enemy_Stand");
		}
		break;
		case ENEMY_TYPE::RangeB_Enemy: 
		{
			GetAnimator()->Play(L"RangeB_Enemy_Stand");
		}
		break;
	}
}
