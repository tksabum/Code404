#pragma once
#include "Component.h"

#include "BJMath.h"
#include "Vector3D.h"

class Patrol :
    public BJEngine::Component
{
private:
    std::vector<BJEngine::Math::Vector3D> patrolPoint;
    int maxPatrolCount;
    int patrolCount;
    float moveSpeed = 64.f;
    BJEngine::Math::Vector3D originPos;

public:
    void SetPatrolPoint(const std::vector<BJEngine::Math::Vector3D>& patrolpoint);
    void SetMoveSpeed(float movespeed);

private:
    void Move(BJEngine::Math::Vector3D startpos, BJEngine::Math::Vector3D& endpos);

public:
    virtual void Awake() override;
    virtual void Update() override;
};

