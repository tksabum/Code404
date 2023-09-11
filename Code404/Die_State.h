#pragma once
#include "State.h"

class Die_State :
    public State
{
private:
	float dieTime;
	float totalDieTime;

	bool isDie = false;

public:
	bool GetIsDie();

public:
	virtual void Update() override;
	virtual void Enter() override;
	virtual void Exit() override;

public:
	Die_State(AI* ai, float dietime);
	~Die_State();
};

