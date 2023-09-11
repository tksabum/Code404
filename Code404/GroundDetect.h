#pragma once
#include "Component.h"

namespace BJEngine
{
	class Collider;
}

class Trace_State;
class Patrol_State;
class AI;

class GroundDetect
	: public BJEngine::Component
{
private:
	BJEngine::GameObject* parentObject;

	Trace_State* traceState;
	Patrol_State* patrolState;
	AI* ai;

public:
	virtual void Awake() override;
	virtual void OnTriggerExit(BJEngine::Collider* other) override;
	virtual void OnTriggerEnter(BJEngine::Collider* other) override;
	virtual void OnTriggerStay(BJEngine::Collider* other) override;
};

