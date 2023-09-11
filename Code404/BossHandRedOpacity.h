#pragma once
#include "Component.h"

namespace BJEngine
{
    class SpriteRenderer;
}

using namespace BJEngine;

class BossHandRedOpacity :
    public BJEngine::Component
{
private:
    float* curHP;
    float* maxHP;
    SpriteRenderer* renderer;

public:
    void SetHP(float* maxhp, float* curhp);

public:
    virtual void Awake() override;
    virtual void Update() override;

};

