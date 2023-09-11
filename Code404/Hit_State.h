#pragma once
#include "State.h"



class Hit_State :
    public State
{
private:
	bool isAttacked;
	float hitTime = 0.5f;
	float totalHitTime;

public:
	bool GetAttacked();

public:
	virtual void Update() override;
	virtual void Enter() override;
	virtual void Exit() override;

public:
	Hit_State(AI* ai);
	~Hit_State();
};

