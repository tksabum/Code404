#pragma once
#include "Component.h"

enum class RANGE_TYPE
{
	NONE,
	RECOGNIZE,
	TRACETARGET,
	ATTACK
};

namespace BJEngine
{
	class Collider;
}

class AI;
class Enemy;
class Trace_State;
class Patrol_State;
class NormalAttack_State;
class Die_State;

class RangeDetect :
	public BJEngine::Component
{
private:
	RANGE_TYPE rangeType= RANGE_TYPE::NONE;
	AI* ai;	// 오브젝트의 AI 컴포넌트를 참조
	Enemy* enemyComponent;

	BJEngine::GameObject* parentObject;
	Trace_State* traceState;
	Patrol_State* patrolState;
	NormalAttack_State* attackState;
	Die_State* dieState;

public:
	void SetRangeType(RANGE_TYPE rangetype);
	void SetAI(AI* ai);

public:
	virtual void Awake() override;

	virtual void OnTriggerEnter(BJEngine::Collider* other) override;
	virtual void OnTriggerStay(BJEngine::Collider* other) override;
	virtual void OnTriggerExit(BJEngine::Collider* other) override;
};

