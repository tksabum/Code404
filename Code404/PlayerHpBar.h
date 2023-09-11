#pragma once
#include "Component.h"

class PlayerHpBar :
    public BJEngine::Component
{
private:
    float* playerMaxHp;
    float* playerCurHp;

public:
    void SetStatus(float* playermaxhp, float* playercurhp);

public:
    virtual void Update() override;

};

