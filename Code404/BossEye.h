#pragma once
#include "Component.h"

namespace BJEngine
{
    class GameObject;
}

class BossEye :
    public BJEngine::Component
{
private:
    BJEngine::GameObject* target;

    float maxDistance = 6.f;

public:
    void SetTarget(BJEngine::GameObject* targetobject);


public:
    virtual void Update() override;
};

