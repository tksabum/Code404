#pragma once
#include "State.h"

#include "Vector2D.h"

namespace BJEngine
{
	class GameObject;
}

enum class TRACE_DIRECTION
{
	LEFT,
	RIGHT
};

class Trace_State :
    public State
{
private:
	BJEngine::GameObject* target;
	TRACE_DIRECTION traceDirection = TRACE_DIRECTION::LEFT;
	bool isGround = true;
	bool isTrace = false;
	float traceSpeed;

public:
	BJEngine::GameObject* GetTargetObject();
	void SetTarget(BJEngine::GameObject* targetobj);

	void SetTraceDirection(TRACE_DIRECTION tracedirection);
	TRACE_DIRECTION GetTraceDirection();

	void SetIsGround(bool isground);

	bool GetIstrace();
	bool GetIsGround();

public:
	virtual void Update() override;
	virtual void Enter() override;
	virtual void Exit() override;

public:
	Trace_State(AI* ai, BJEngine::GameObject* targetobject, float tracespeed);
	~Trace_State();
};

