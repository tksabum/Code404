#pragma once
#include "State.h"

class Idle_State :
    public State
{
private:
    float idleTime = 4.f;
    float totalIdleTime;

public:
    virtual void Update() override;
    virtual void Enter() override;

public:
    Idle_State(AI* ai, float idletime);
    ~Idle_State();
};

