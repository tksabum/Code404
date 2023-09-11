#pragma once
#include "State.h"

#include "Vector2D.h"

namespace BJEngine
{
	class GameObject;
}



enum class PATROL_DIRECTION
{
	LEFT,
	RIGHT
};

class Patrol_State :
    public State
{
private:
	PATROL_DIRECTION patrolDirection;
	BJEngine::GameObject* target;

	float patrolTime= 4.f;
	float totalPatrolTime;

	float patrolSpeed;

	bool isPatrol = false;
	bool isRandom = true;

public:
	void SetPatrolDirection(PATROL_DIRECTION patroldirection);
	PATROL_DIRECTION GetPatrolDirection();

	void SetIsRandom(bool israndom);

	bool GetIsPatrol();

private:


public:
	virtual void Update() override;
	virtual void Enter() override;
	virtual void Exit() override;

public:
	Patrol_State(AI* ai, BJEngine::GameObject* targetobject,float patroltime, float patrolspeed);
	~Patrol_State();
};

