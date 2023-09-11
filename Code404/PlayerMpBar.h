#pragma once
#include "Component.h"
class PlayerMpBar :
    public BJEngine::Component
{
private:
    float* playerMaxMp;
    float* playerCurMp;

public:
    virtual void Update() override;

public:
    void SetStatus(float* playermaxmp, float* playercurmp);
};

