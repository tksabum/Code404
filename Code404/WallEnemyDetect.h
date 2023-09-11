#pragma once
#include "Component.h"

using namespace BJEngine;

namespace BJEngine
{
	class Collider;
}

class Trace_State;
class Patrol_State;
class AI;

class WallEnemyDetect :
    public Component
{
private:
	BJEngine::GameObject* parentObject;
	Trace_State* traceState;
	Patrol_State* patrolState;
	AI* ai;

public:
	virtual void Awake() override;
	virtual void OnTriggerEnter(BJEngine::Collider* other) override;

};

