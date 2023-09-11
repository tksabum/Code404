#pragma once
#include "Component.h"

class AttackCollider :
    public BJEngine::Component
{
private:
    float totalTime;
    float damage;

public:
    void SetDamage(float damage);

public:
    virtual void Update() override;


    virtual void OnTriggerEnter(BJEngine::Collider* other) override;

};

