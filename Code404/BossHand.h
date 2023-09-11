#pragma once
#include "Component.h"

#include "BJMath.h"
#include "Vector3D.h"

namespace BJEngine
{
    class Rigidbody2D;
    class Collider;
}
class Boss;
class Enemy;

enum class HAND_TYPE
{
    LEFT = 1,
    RIGHT = -1,
};


class BossHand :
    public BJEngine::Component
{
private:
    Boss* bossComponent;
    Enemy* enemyComponent;

    HAND_TYPE handType;

    float collisionDamage;
    float takeDownDamage;

    BJEngine::Math::Vector3D originPosition;
    BJEngine::Math::Vector3D prevPosition;

    BJEngine::Rigidbody2D* rigid;

    bool isCameraShake;

    bool isHandShake;
    float handShakeTime = 0.5;
    float totalHandShakeTime;

    bool isTakeDown;
    bool isGround;
    bool isHorizontalMove = true;
    bool isVerticalMove = false;
    int takeDownCount;
    int maxTakeDownCount = 3;
    float horizontalOffset = 300.f;
    float verticalOffset = 80.f;
    float takeDownMoveSpeed = 150.f;

    bool isSweepHand;
    bool isRightHandSweep;
    std::vector< BJEngine::Math::Vector3D> vecPatrol;
    int patrolIndex;
    int maxPatrolIndex;

    float damage = 50;

    float groundWaitTime = 1.f;
    float totalGroundWaitTime;

	float totalEffectTime = 0.f;
	float nextEffectTime = 0.1f;
    int maxEffectCount = 5;
    int curEffectCount = 0;

    std::vector<BJEngine::Math::Vector3D> vecTakeDown;
	int takeDownIndex;
	int maxTakeDownIndex;

    bool isReadyHandShot = false;
    int randomPos;
    float handShotSpeed = 100.f;
    int platformTriggerCount = 0;
    bool isRealShot = false;
    int callCount = 0;
    int sweepCount=-1;
    int maxSweepCount = 4;

public:
    int GetCallCount();

    void SetHandType(HAND_TYPE handtype);

    void SetIsTakeDown(bool istakedown);
    void SetIsSweep(bool issweephand);
    void TakeDown();

    float GetMoveSpeed();
    int GetTriggerCount();
   

    Enemy* GetEnemyComponent();

private:
    void Move(BJEngine::Math::Vector3D startpos, BJEngine::Math::Vector3D endpos, float movespeed);
    void TakeDownMove(BJEngine::Math::Vector3D startpos, BJEngine::Math::Vector3D endpos, float movespeed);
    bool HandSweepMove(BJEngine::Math::Vector3D startpos, BJEngine::Math::Vector3D endpos, float movespeed);
    void CheckDie();
    void MakeEffect();
    void SetTakeDownPoint();
    void HandSweepLoop();
    void SweepHand(int positionindex);

    
public:
    virtual void Awake() override;
    virtual void Update() override;

    virtual void OnTriggerEnter(BJEngine::Collider* other) override;
    virtual void OnTriggerExit(BJEngine::Collider* other) override;
    virtual void OnDisable() override;

};

