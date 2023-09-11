#include <vector>
#include "Patrol.h"

#include "CoreSystem.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TimeController.h"

#include "GameObject.h"
#include "Transform.h"

using namespace BJEngine;
using namespace BJEngine::Math;

void Patrol::SetPatrolPoint(const std::vector<BJEngine::Math::Vector3D>& patrolpoint)
{
	patrolPoint = patrolpoint;
	this->maxPatrolCount = patrolPoint.size();
}


void Patrol::SetMoveSpeed(float movespeed)
{
	moveSpeed = movespeed;
}

void Patrol::Move(BJEngine::Math::Vector3D startpos, BJEngine::Math::Vector3D& endpos)
{
	Vector3D dir = endpos - startpos;
	dir = dir.GetUnitVector();
	if (abs(startpos.x - endpos.x) < 5.f
		&& abs(startpos.y - endpos.y) < 5.f
		&& abs(startpos.z - endpos.z) < 5.f)
	{
		startpos = endpos;
		patrolCount++;
	}
	else
	{
		startpos += dir * moveSpeed * CoreSystem::GetInstance().GetSceneManager()->GetCurrentScene()->GetTimeController()->GetDeltaTime();
	}
	GetGameObject()->GetTransform()->SetLocalPosition(startpos);
}

void Patrol::Awake()
{
	originPos = GetGameObject()->GetTransform()->GetLocalPosition();
	//Vector3D patrolPoint1 = Vector3D(originPos.x, originPos.y - 200, originPos.z);
	//Vector3D patrolPoint2 = Vector3D(originPos.x, originPos.y + 450, originPos.z);

	Vector3D patrolPoint1 = Vector3D(originPos.x - 1000, originPos.y, originPos.z);
	Vector3D patrolPoint2 = Vector3D(originPos.x + 1000, originPos.y, originPos.z);

	patrolPoint.push_back(patrolPoint1);
	patrolPoint.push_back(patrolPoint2);
	maxPatrolCount = patrolPoint.size();
}

void Patrol::Update()
{
	if (patrolCount < maxPatrolCount)
	{
		Move(GetGameObject()->GetTransform()->GetLocalPosition(), patrolPoint[patrolCount]);
	}
	else
	{
		patrolCount = 0;
	}
}
