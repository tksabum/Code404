#pragma once
#include "Component.h"

class BossNormalBullet :
    public BJEngine::Component
{
private:
    float damage = 10;
    float releaseTime = 5.f;
    float totalReleaseTime;

public:
    void SetBulletDamage(float damage);

public:
    virtual void Update() override;

    virtual void OnTriggerEnter(BJEngine::Collider* other) override;
    virtual void OnDisable() override;
};

