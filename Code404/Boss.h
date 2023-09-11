#pragma once
#include "Component.h"

#include "Vector3D.h"

enum class PHASE_STATE
{
    ONE,
    TWO
};

enum class BOSS_STATE
{
    IDLE,
    HANDTAKEDOWN,
    HANDSWEEP,
    HEAD_TAKEDOWN,
    DIE
};

class BossHand;
class BossEye;
class Enemy;

namespace BJEngine
{
    class Rigidbody2D;
    class Image;
}

class Boss :
    public BJEngine::Component
{
private:
    BJEngine::Image* portrait;

    BJEngine::GameObject* bossHPBar;

    Enemy* enemyComponent;
    float damage = 50;

    BJEngine::GameObject* target;
    BJEngine::GameObject* leftHand;
    BossHand* leftHandComponent;
    BJEngine::GameObject* rightHand;
    BossHand* rightHandComponent;
	BJEngine::GameObject* bossEye;

    BJEngine::GameObject* crossLaserParent;
    BJEngine::GameObject* patrolLaser;
    BJEngine::GameObject* timeLaserParent;

    PHASE_STATE phaseState = PHASE_STATE::ONE;
    BOSS_STATE bossState = BOSS_STATE::IDLE;

    float takeDownDamage;   // 내려찍기 대미지
    float laserDamage;   // 레이저 대미지

    bool isPattern = false; // 현재 패턴을 진행중인가
    float totalpatternTime;

    bool isRightHandSweep;

    BJEngine::Rigidbody2D* bossRigid;
    BJEngine::Math::Vector3D prevPos;
    bool isCalPrevPos;
    bool isGround;
    bool isTakeDown;
    float takeDownCoolTime = 6.f;
    float calCoolTime;
    float sleepTime = 1.f;
    float calSleepTime;

    float totalShootTime;
    float shootTime = 4.f;
    bool isSummonsBullet = false;

    float patternTime = 2.f;

    float phaseChangeTime = 4.f;
    float totalChangeTime = 0;

    float groundWaitTime = 2.5f;
    float totalGroundWaitTime;

    bool isShotLaser = false;

    bool isTrace = false;

	float totalEffectTime = 0.f;
	float nextEffectTime = 0.1f;
	int maxEffectCount = 50;
	int curEffectCount = 0;

    bool isDieEffect = false;

    bool isLeftFinish = true;
    bool isRightFinish = true;

    float rotateSpeed = 20;
    float traceSpeed = 200.f;

    bool isCameraShake = false;

    bool isDie = false;

public:
    void SetTarget(BJEngine::GameObject* target);
    void SetIsPattern(bool ispattern);
    void SetIsRightHandSweep(bool isrighthandsweep);

    void SetLeftFinish(bool leftfinish);
    void SetRightFinish(bool rightfinish);

    PHASE_STATE GetPhaseState();

    Enemy* GetEnemyComponent();

    void SetIsCameraShake(bool iscamerashake);
    bool GetIsCameraShake();

public:
    virtual void Awake() override;
    virtual void Update() override;

    virtual void OnTriggerEnter(BJEngine::Collider* other) override;

private:
    void PhaseOne();
    void PhaseTwo();

    void CheckDie();
    void ChangePhase();

    void ChangeHPBarScale();

    // 페이지1에 사용
    void ShotBullet();
    void HandTakeDown();
    void HandSweep();
    void RandomRocket();

    // 페이지2에 사용
    void HeadTakeDown();
    void TraceTarget();
    void CloseEye();

    void CheckState();

    void MakeDieEffect();

    friend BossHand;
    friend BossEye;
};
