#include "Patrol_State.h"

#include "Trace_State.h"

#include "CoreSystem.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TimeController.h"

#include "BJMath.h"
#include "GameObject.h"
#include "Transform.h"
#include "Vector3D.h"
#include "LineCollider.h"
#include "LineRenderer.h"

#include "AI.h"

#include "Animator.h"

#include <random>

using namespace std;
using namespace BJEngine;
using namespace BJEngine::Math;

Patrol_State::Patrol_State(AI* ai, GameObject* targetobject, float patroltime, float patrolspeed)
	: State(ai)
	, patrolTime(patroltime)
	, patrolDirection(PATROL_DIRECTION::LEFT)
	, target(targetobject)
	, patrolSpeed(patrolspeed)
{

}

Patrol_State::~Patrol_State()
{

}

void Patrol_State::SetPatrolDirection(PATROL_DIRECTION patroldirection)
{
	patrolDirection = patroldirection;
}

PATROL_DIRECTION Patrol_State::GetPatrolDirection()
{
	PATROL_DIRECTION temp = patrolDirection;
	return patrolDirection;
}

void Patrol_State::SetIsRandom(bool israndom)
{
	isRandom = israndom;
}

bool Patrol_State::GetIsPatrol()
{
	return isPatrol;
}

void Patrol_State::Update()
{
	State::Update();

	if (isPatrol)
	{
		AI* ai = GetAI();

		float deltaTime = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetDeltaTime();
		totalPatrolTime += deltaTime;
		// 순찰 시간 다 되면 가만히 있기
		if (totalPatrolTime > patrolTime)
		{
			totalPatrolTime = 0.f;
			ai->ChangeState(AI_STATE::IDLE);
		}

		// 이동 방향에 따른 위치 변경
		Vector3D pos = ai->GetGameObject()->GetTransform()->GetLocalPosition();
		if (patrolDirection == PATROL_DIRECTION::LEFT)
		{
			pos += -patrolSpeed * deltaTime;
		}
		else
		{
			pos += patrolSpeed * deltaTime;
		}
		GetAI()->GetGameObject()->GetTransform()->SetLocalPosition(pos);
	}
}

void Patrol_State::Enter()
{
	isPatrol = true;

	AI* ai = GetAI();
	GameObject* owner = ai->GetGameObject();

	ENEMY_TYPE type = ai->GetEnemyType();
	switch (type)
	{
		case ENEMY_TYPE::MeleeA_Enemy:
		{
			GetAnimator()->Play(L"MeleeA_Enemy_Walk");
		}
		break;

		case ENEMY_TYPE::MeleeB_Enemy:
		{
			GetAnimator()->Play(L"MeleeB_Enemy_Walk");
		}
			break;
		case ENEMY_TYPE::RangeA_Enemy:
		{
			GetAnimator()->Play(L"RangeA_Enemy_Walk");
		}
		break;
		case ENEMY_TYPE::RangeB_Enemy:
		{
			GetAnimator()->Play(L"RangeB_Enemy_Walk");
		}
		break;
	}

	totalPatrolTime = 0.f;

	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> range(1, 2);
	int randomDirection = range(mt);

	// 랜덤으로 방향을 바꾼다면 아래 코드 실행
	if (isRandom)
	{
		if (randomDirection == 1)
		{
			patrolDirection = PATROL_DIRECTION::LEFT;
		}
		else if (randomDirection == 2)
		{
			patrolDirection = PATROL_DIRECTION::RIGHT;
		}
	}
	// 순찰을 왼쪽으로 하는데 scale.x값이 음수라면 양수로
	if (patrolDirection == PATROL_DIRECTION::LEFT)
	{
		if (owner->GetTransform()->GetLocalScale().x < 0)
		{
			owner->GetTransform()->SetLocalScale(Vector3D(owner->GetTransform()->GetLocalScale().x * -1, owner->GetTransform()->GetLocalScale().y, owner->GetTransform()->GetLocalScale().z));
		}
	}
	else
	{
		// 순찰을 오른쪽으로 하는데 scale.x값이 양수라면 음수로
		if (owner->GetTransform()->GetLocalScale().x > 0)
		{
			owner->GetTransform()->SetLocalScale(Vector3D(owner->GetTransform()->GetLocalScale().x * -1, owner->GetTransform()->GetLocalScale().y, owner->GetTransform()->GetLocalScale().z));
		}
	}

	isRandom = true;
}

void Patrol_State::Exit()
{
	isPatrol = false;
	Vector3D targetPos = target->GetTransform()->GetLocalPosition();
	AI* ai = GetAI();
	GameObject* owner = ai->GetGameObject();
	// 왼쪽으로 진행
	if (targetPos.x < owner->GetTransform()->GetLocalPosition().x)
	{
		patrolDirection = PATROL_DIRECTION::LEFT;
	}
	else
	{
		// 오른쪽으로 진행
		patrolDirection = PATROL_DIRECTION::RIGHT;
	}
}
