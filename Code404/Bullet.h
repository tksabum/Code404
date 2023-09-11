#pragma once
#include "Component.h"

#include "BJMath.h"
#include "Vector3D.h"
#include "Vector2D.h"
#include "Code404.h"

using namespace BJEngine;
using namespace BJEngine::Math;

namespace BJEngine
{
    class Collider;
}

class ParryingSystem;

class Bullet :
    public Component
{
private:
    float ableTime = 5;
    float totalAbleTime;

    float damage;
    float moveSpeed;
    BJEngine::Math::Vector2D direction;

    bool isParrying;
    ParryingSystem* parryingSystem;

public:
    void SetDamage(float damage);
    void SetSpeed(float speed);
    void SetDirection(BJEngine::Math::Vector2D direction);
    
    void Parrying();

public:
    void Awake();
    void OnEnable();

    virtual void Update() override;

    virtual void OnTriggerEnter(BJEngine::Collider* other);

    virtual void OnDisable();
};

