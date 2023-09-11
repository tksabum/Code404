#include "GroundDetect.h"

#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"
#include "Vector3D.h"
#include "BJMath.h"

#include "Patrol_State.h"
#include "Trace_State.h"

#include "AI.h"

using namespace BJEngine;
using namespace BJEngine::Math;

void GroundDetect::Awake()
{
	parentObject = GetGameObject()->GetParent();
	ai = parentObject->GetComponent<AI>();
	traceState = ((Trace_State*)(ai->GetMapState()[AI_STATE::TRACETARGET]));
	patrolState = ((Patrol_State*)(ai->GetMapState()[AI_STATE::PATROL]));
}

void GroundDetect::OnTriggerExit(BJEngine::Collider* other)
{
	if (other->GetGameObject()->GetLayerName() == L"Platform" )
	{
		traceState->SetIsGround(false);
		if (patrolState)
		{
			if (patrolState->GetIsPatrol())
			{
				switch (patrolState->GetPatrolDirection())
				{
					case PATROL_DIRECTION::LEFT:
					{
						if (parentObject->GetTransform()->GetLocalScale().x > 0)
						{
							parentObject->GetTransform()->SetLocalScale(Vector3D(parentObject->GetTransform()->GetLocalScale().x * -1,
								parentObject->GetTransform()->GetLocalScale().y,
								parentObject->GetTransform()->GetLocalScale().z));
						}

						patrolState->SetPatrolDirection(PATROL_DIRECTION::RIGHT);
					}
					break;
					case PATROL_DIRECTION::RIGHT:
					{
						if (parentObject->GetTransform()->GetLocalScale().x < 0)
						{
							parentObject->GetTransform()->SetLocalScale(Vector3D(parentObject->GetTransform()->GetLocalScale().x * -1,
								parentObject->GetTransform()->GetLocalScale().y,
								parentObject->GetTransform()->GetLocalScale().z));
						}
						patrolState->SetPatrolDirection(PATROL_DIRECTION::LEFT);
					}
					break;
				}
			}
		}
	}
}

void GroundDetect::OnTriggerEnter(BJEngine::Collider* other)
{
	
}

void GroundDetect::OnTriggerStay(BJEngine::Collider* other)
{
	if (other->GetGameObject()->GetLayerName() == L"Platform")
	{
		if (traceState)
		{
			traceState->SetIsGround(true);
		}
	}
}
