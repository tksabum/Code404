#pragma once
#include "Component.h"

class laser :
    public BJEngine::Component
{
private:
    float damage = 10;

public:
    void SetLaserDamage(float damage);

public:
    virtual void OnTriggerEnter(BJEngine::Collider* other);
    virtual void OnTriggerStay(BJEngine::Collider* other);
};

