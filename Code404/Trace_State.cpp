#include "Trace_State.h"
#include "BJMath.h"
#include "Vector3D.h"

#include "AI.h"
#include "GameObject.h"
#include "Transform.h"

#include "CoreSystem.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TimeController.h"

#include "Patrol_State.h"

#include "Animator.h"

using namespace std;
using namespace BJEngine;
using namespace BJEngine::Math;


Trace_State::Trace_State(AI* ai, BJEngine::GameObject* targetobject, float tracespeed)
	: State(ai)
	, target(targetobject)
	, traceSpeed(tracespeed)
{

}

Trace_State::~Trace_State()
{

}

BJEngine::GameObject* Trace_State::GetTargetObject()
{
	return target;
}

void Trace_State::SetTarget(BJEngine::GameObject* targetobj)
{
	target = targetobj;
}

void Trace_State::SetTraceDirection(TRACE_DIRECTION tracedirection)
{
	traceDirection = tracedirection;
}

TRACE_DIRECTION Trace_State::GetTraceDirection()
{
	return traceDirection;
}

void Trace_State::SetIsGround(bool isground)
{
	isGround = isground;
}

bool Trace_State::GetIstrace()
{
	return isTrace;
}

bool Trace_State::GetIsGround()
{
	return isGround;
}

void Trace_State::Update()
{
	if (isGround && isTrace)
	{
		AI* ai = GetAI();
		GameObject* owner = ai->GetGameObject();
		Vector3D ownerpos = owner->GetTransform()->GetLocalPosition();
		Vector3D dir = target->GetTransform()->GetLocalPosition() - GetAI()->GetGameObject()->GetTransform()->GetLocalPosition();
		dir.y = 0.f;
		dir.z = 0.f;
		ownerpos += dir.GetUnitVector() * traceSpeed * CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetDeltaTime();
		GetAI()->GetGameObject()->GetTransform()->SetLocalPosition(ownerpos);
		map<AI_STATE, State*>& mapTemp = ai->GetMapState();

		if (target->GetTransform()->GetLocalPosition().x < ownerpos.x)
		{
			((Patrol_State*)mapTemp[AI_STATE::PATROL])->SetPatrolDirection(PATROL_DIRECTION::LEFT);
			traceDirection = TRACE_DIRECTION::LEFT;
		}
		else
		{
			((Patrol_State*)mapTemp[AI_STATE::PATROL])->SetPatrolDirection(PATROL_DIRECTION::RIGHT);
			traceDirection = TRACE_DIRECTION::RIGHT;
		}

		if (traceDirection == TRACE_DIRECTION::LEFT)
		{
			if (owner->GetTransform()->GetLocalScale().x < 0)
			{
				owner->GetTransform()->SetLocalScale(Vector3D(owner->GetTransform()->GetLocalScale().x * -1, owner->GetTransform()->GetLocalScale().y, owner->GetTransform()->GetLocalScale().z));
			}
		}
		else
		{
			// 추격을 오른쪽으로 하는데 scale.x가 양수면 음수로
			if (owner->GetTransform()->GetLocalScale().x > 0)
			{
				owner->GetTransform()->SetLocalScale(Vector3D(owner->GetTransform()->GetLocalScale().x * -1, owner->GetTransform()->GetLocalScale().y, owner->GetTransform()->GetLocalScale().z));
			}
		}
	}
}

void Trace_State::Enter()
{
	State::Update();

	isTrace = true;

	AI* ai = GetAI();
	GameObject* owner = ai->GetGameObject();
	Vector3D ownerpos = owner->GetTransform()->GetLocalPosition();

	ENEMY_TYPE type = ai->GetEnemyType();

	switch (type)
	{
		case ENEMY_TYPE::MeleeA_Enemy:
		{
			if (GetAnimator()->GetAnimationName() != L"MeleeA_Enemy_Walk")
			{
				GetAnimator()->Play(L"MeleeA_Enemy_Walk");
			}
		}
		break;
		case ENEMY_TYPE::MeleeB_Enemy:
		{
			if (GetAnimator()->GetAnimationName() != L"MeleeB_Enemy_Walk")
			{
				GetAnimator()->Play(L"MeleeB_Enemy_Walk");
			}
		}
			break;
		case ENEMY_TYPE::RangeA_Enemy:
		{
			if (GetAnimator()->GetAnimationName() != L"RangeA_Enemy_Walk")
			{
				GetAnimator()->Play(L"RangeA_Enemy_Walk");
			}
		}
		break;
		case ENEMY_TYPE::RangeB_Enemy:
		{
			if (GetAnimator()->GetAnimationName() != L"RangeB_Enemy_Walk")
			{
				GetAnimator()->Play(L"RangeB_Enemy_Walk");
			}
		}
		break;
	}

	std::map<AI_STATE, State*> mapTemp = ai->GetMapState();

	Vector3D targetPos = target->GetTransform()->GetLocalPosition();

	if (targetPos.x < ownerpos.x)
	{
		traceDirection = TRACE_DIRECTION::LEFT;
	}
	else
	{
		traceDirection = TRACE_DIRECTION::RIGHT;
	}

	// 추격을 왼쪽으로 하는데 scale.x가 음수면 양수로
	if (traceDirection == TRACE_DIRECTION::LEFT)
	{
		if (owner->GetTransform()->GetLocalScale().x < 0)
		{
			owner->GetTransform()->SetLocalScale(Vector3D(owner->GetTransform()->GetLocalScale().x * -1, owner->GetTransform()->GetLocalScale().y, owner->GetTransform()->GetLocalScale().z));
		}
	}
	else
	{
		// 추격을 오른쪽으로 하는데 scale.x가 양수면 음수로
		if (owner->GetTransform()->GetLocalScale().x > 0)
		{
			owner->GetTransform()->SetLocalScale(Vector3D(owner->GetTransform()->GetLocalScale().x * -1, owner->GetTransform()->GetLocalScale().y, owner->GetTransform()->GetLocalScale().z));
		}
	}
}

void Trace_State::Exit()
{
	isTrace = false;
}
