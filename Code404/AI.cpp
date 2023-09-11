#include "AI.h"

#include "Idle_State.h"
#include "Trace_State.h"
#include "Patrol_State.h"
#include "NormalAttack_State.h"
#include "Die_State.h"

#include "Collider.h"

#include "GameObject.h"
#include "Transform.h"
#include "ParryingSystem.h"
#include "ObjectPoolSystem.h"
#include "EffectPlayer.h"

AI::AI()
	: mapState{}
	, curState(nullptr)
{

}

AI::~AI()
{
	for (auto& item : mapState)
	{
		delete item.second;
	}
}

void AI::CreateState(AI_STATE aistate, State* paistate)
{
	switch (aistate)
	{
		case AI_STATE::IDLE:
		{
			mapState.insert({AI_STATE::IDLE, paistate });
			curState = mapState[AI_STATE::IDLE];
		}
			break;
		case AI_STATE::TRACETARGET:
		{
			mapState.insert({ AI_STATE::TRACETARGET, paistate });
		}
			break;
		case AI_STATE::NORMALATTACK:
		{
			mapState.insert({ AI_STATE::NORMALATTACK, paistate });
		}
		break;
		case AI_STATE::PATROL:
		{
			mapState.insert({ AI_STATE::PATROL, paistate });
		}
			break;
		case AI_STATE::DIE:
		{
			mapState.insert({ AI_STATE::DIE, paistate });
			//curState = mapState[AI_STATE::DIE];
		}
			break;
		case AI_STATE::HIT:
		{
			mapState.insert({ AI_STATE::HIT, paistate });
		}
		break;
	}
}

void AI::ChangeState(AI_STATE nextstate)
{
	// 같은 상태로 변하려 하면 리턴
	if (curState == mapState[nextstate])
	{
		return;
	}
	
	// 공격중 공격받는다면 패링 (근거리 적 한정)
	if (curState == mapState[AI_STATE::NORMALATTACK] && nextstate == AI_STATE::HIT && (enemyType == ENEMY_TYPE::MeleeA_Enemy || enemyType == ENEMY_TYPE::MeleeB_Enemy))
	{
		if (curState->IsParryingTime() == true)
		{
			// 패링 이펙트
			GameObject* parryingeffect = ObjectPoolSystem::GetInstance().PopFromPool(L"Effect");
			parryingeffect->SetActive(true);

			EffectPlayer* effectanimator = parryingeffect->GetComponent<EffectPlayer>();
			effectanimator->Reset(L"Animation_Parrying_Effect");

			parryingeffect->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetWorldPosition() + Vector2D(0.0f, 0.0f));
			parryingeffect->GetTransform()->SetLocalScale(Vector3D(2.0f, 2.0f, 1.0f));

			parryingSystem->Parrying();
		}
	}

	curState->Exit();
	prevState = curState;
	curState = mapState[nextstate];
	curState->Enter();
}

State* AI::GetState(AI_STATE aistate)
{
	if (mapState[aistate])
	{
		return mapState[aistate];
	}
	else
	{
		return nullptr;
	}
}

void AI::SetEnemyType(ENEMY_TYPE type)
{
	enemyType = type;
}

ENEMY_TYPE AI::GetEnemyType()
{
	return enemyType;
}

State* AI::GetPrevState()
{
	return prevState;
}

std::map<AI_STATE, State*>& AI::GetMapState()
{
	return mapState;
}

void AI::Awake()
{
	GameObject* parryingsystemobject = CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetGameObject(L"ParringSystem");
	if (parryingsystemobject != nullptr)
	{
		parryingSystem = parryingsystemobject->GetComponent<ParryingSystem>();
	}
}

void AI::Update()
{
	curState->Update();

	for (auto& i : mapState)
	{
		i.second->AlwaysUpdate();
	}
}


//void AI::OnTriggerStay(BJEngine::Collider* other)
//{
//	curState = mapState[AI_STATE::TRACETARGET];
//	((Trace_State*)(curState))->SetTarget(other->GetGameObject());
//}
//
//void AI::OnTriggerExit(BJEngine::Collider* other)
//{
//	curState = mapState[AI_STATE::IDLE];
//}
