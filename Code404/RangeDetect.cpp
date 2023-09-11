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
						// �ν� ������ ������
						// ����� ���� �̵� ���⿡ ���� �б⹮
						if (patrolState->GetPatrolDirection() == PATROL_DIRECTION::LEFT)
						{
							// ����� �������� �������̶��
							if (targetPos.x < ownerPos.x)
							{
								// �տ� �����Ƿ� �߰�
								// ((Trace_State*)(mapTemp[AI_STATE::TRACETARGET]))->SetTraceDirection(TRACE_DIRECTION::LEFT);
								ai->ChangeState(AI_STATE::TRACETARGET);
							}
							else
							{
								// �ڿ� �����Ƿ� ����ǥ�� ������ �ϱ�
							}
						}
						else
						{
							// ����� ���������� �������̶��
							if (targetPos.x > ownerPos.x)
							{
								// �տ� �����Ƿ� �߰ݻ���
								ai->ChangeState(AI_STATE::TRACETARGET);
							}
							else
							{
								// �ڿ� �����Ƿ� ����ǥ�� ������ �ϱ�
							}
						}
					}
				}
				break;
				case RANGE_TYPE::TRACETARGET:
				{
					// �߰� �����ȿ� ���´ٸ�
					ai->ChangeState(AI_STATE::TRACETARGET);
				}
				break;
				case RANGE_TYPE::ATTACK:
				{
					// ���� �����ȿ� ���´ٸ�
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
					// �ν� ������ ������
					// �����ϰ� ���� �� �� Idle State��
					if (traceState)
					{
						if (traceState->GetIstrace())
						{
							// �������ε� �÷��� ���� ������ ���
							if (!traceState->GetIsGround())
							{
								patrolState->SetIsRandom(false);
								// ���� ������ �����̶�� ���� ��Ʈ���� ����������
								if (traceState->GetTraceDirection() == TRACE_DIRECTION::LEFT)
								{
									patrolState->SetPatrolDirection(PATROL_DIRECTION::RIGHT);
								}
								else
								{
									// ���� ������ �����̶�� ���� ��Ʈ���� ��������
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
