#include "RangeDetect.h"

#include "BJMath.h"

#include "GameObject.h"
#include "Transform.h"
#include "Vector3D.h"
#include "Collider.h"

#include "AI.h"
#include "Patrol_State.h"
#include "Trace_State.h"
#include "Die_State.h"
#include "NormalAttack_State.h"
#include "Hit_State.h"

#include "Enemy.h"

using namespace std;
using namespace BJEngine;
using namespace BJEngine::Math;

void RangeDetect::SetRangeType(RANGE_TYPE rangetype)
{
	rangeType = rangetype;
}

void RangeDetect::SetAI(AI* ai)
{
	this->ai = ai;
}

void RangeDetect::Awake()
{
	enemyComponent = GetGameObject()->GetParent()->GetComponent<Enemy>();
	map<AI_STATE, State*>& mapTemp = ai->GetMapState();
	parentObject = GetGameObject()->GetParent();
	patrolState = ((Patrol_State*)(mapTemp[AI_STATE::PATROL]));
	traceState = ((Trace_State*)(mapTemp[AI_STATE::TRACETARGET]));
	attackState = ((NormalAttack_State*)(mapTemp[AI_STATE::NORMALATTACK]));
	dieState = ((Die_State*)(mapTemp[AI_STATE::DIE]));
}

void RangeDetect::OnTriggerEnter(BJEngine::Collider* other)
{
	Vector3D targetPos = other->GetGameObject()->GetTransform()->GetLocalPosition();
	Vector3D ownerPos = parentObject->GetTransform()->GetLocalPosition();

	if (enemyComponent->IsAttacked())
	{
		ai->ChangeState(AI_STATE::HIT);
		return;
	}

	if (!dieState->GetIsDie())
	{
		if (other->GetGameObject()->GetLayerName() == L"Player")
		{
			switch (rangeType)
			{
				case RANGE_TYPE::RECOGNIZE:
				{
				if (traceState)
					{
						// 인식 범위에 들어왔음
						// 잡몹의 현재 이동 방향에 따른 분기문
						if (patrolState->GetPatrolDirection() == PATROL_DIRECTION::LEFT)
						{
							// 잡몹이 왼쪽으로 진행중이라면
							if (targetPos.x < ownerPos.x)
							{
								// 앞에 있으므로 추격
								// ((Trace_State*)(mapTemp[AI_STATE::TRACETARGET]))->SetTraceDirection(TRACE_DIRECTION::LEFT);
								ai->ChangeState(AI_STATE::TRACETARGET);
							}
							else
							{
								// 뒤에 있으므로 물음표만 나오게 하기
							}
						}
						else
						{
							// 잡몹이 오른쪽으로 진행중이라면
							if (targetPos.x > ownerPos.x)
							{
								// 앞에 있으므로 추격상태
								ai->ChangeState(AI_STATE::TRACETARGET);
							}
							else
							{
								// 뒤에 있으므로 물음표만 나오게 하기
							}
						}
					}
				}
				break;
				case RANGE_TYPE::TRACETARGET:
				{
					// 추격 범위안에 들어온다면
					ai->ChangeState(AI_STATE::TRACETARGET);
				}
				break;
				case RANGE_TYPE::ATTACK:
				{
					// 공격 범위안에 들어온다면
					ai->ChangeState(AI_STATE::NORMALATTACK);
				}
				break;
			}
		}
	}
}

void RangeDetect::OnTriggerStay(BJEngine::Collider* other)
{
	if (enemyComponent->IsAttacked())
	{
		ai->ChangeState(AI_STATE::HIT);
		return;
	}

	if (other->GetGameObject()->GetLayerName() == L"Player")
	{
		switch (rangeType)
		{
			case RANGE_TYPE::RECOGNIZE:
			{

			}
			break;
			case RANGE_TYPE::TRACETARGET:
			{
				if (!attackState->GetIsAttack())
				{
					ai->ChangeState(AI_STATE::TRACETARGET);
				}
			}
			break;
			case RANGE_TYPE::ATTACK:
			{
				ai->ChangeState(AI_STATE::NORMALATTACK);
			}
			break;
		}
	}
}

void RangeDetect::OnTriggerExit(BJEngine::Collider* other)
{
	Vector3D targetPos = other->GetGameObject()->GetTransform()->GetLocalPosition();
	Vector3D ownerPos = parentObject->GetTransform()->GetLocalPosition();

	if (enemyComponent->IsAttacked())
	{
		ai->ChangeState(AI_STATE::HIT);
		return;
	}

	if (!dieState->GetIsDie())
	{
		if (other->GetGameObject()->GetLayerName() == L"Player" || other->GetGameObject()->GetLayerName() == L"StealthPlayer")
		{
			switch (rangeType)
			{
				case RANGE_TYPE::RECOGNIZE:
				{
					// 인식 범위를 나가면
					// 추적하고 있을 때 만 Idle State로
					if (traceState)
					{
						if (traceState->GetIstrace())
						{
							// 추적중인데 플랫폼 끝에 도달한 경우
							if (!traceState->GetIsGround())
							{
								patrolState->SetIsRandom(false);
								// 추적 방향이 왼쪽이라면 다음 패트롤은 오른쪽으로
								if (traceState->GetTraceDirection() == TRACE_DIRECTION::LEFT)
								{
									patrolState->SetPatrolDirection(PATROL_DIRECTION::RIGHT);
								}
								else
								{
									// 추적 방향이 오쪽이라면 다음 패트롤은 왼쪽으로
									patrolState->SetPatrolDirection(PATROL_DIRECTION::LEFT);
								}
							}

							ai->ChangeState(AI_STATE::IDLE);
						}
					}
					else
					{
						ai->ChangeState(AI_STATE::IDLE);
					}
					
				}
				break;
				case RANGE_TYPE::TRACETARGET:
				{

				}
				break;
				case RANGE_TYPE::ATTACK:
				{
					attackState->SetIsAttack(false);
					if (!traceState->GetIstrace())
					{
						ai->ChangeState(AI_STATE::IDLE);
					}
				}
				break;
			}
		}
	}
}
