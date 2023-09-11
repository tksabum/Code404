#pragma once
#include "State.h"

namespace BJEngine
{
	class GameObject;
}

enum class Attack_Direction
{
	LEFT,
	RIGHT
};

class NormalAttack_State :
    public State
{
private:
	BJEngine::GameObject* target;

	float attackDelay;
	float totalAttackDelay;

	float rangeAttackFrameTime = 0.6f;
	float meleeAttackFrameTime = 0.8f;
	float totalAttackFrameTime = 0.f;

	bool isAttack = false;
	bool isShot = false;

	BJEngine::GameObject* attackCollider;

	Attack_Direction attackDirection = Attack_Direction::LEFT;

public:
	bool GetIsAttack();
	void SetIsAttack(bool isattack);

	void SetAttackCollider(BJEngine::GameObject* attackcollider);

public:
	virtual void Update() override;
	virtual void Enter() override;
	virtual void Exit() override;

	virtual void AlwaysUpdate() override;

	bool IsParryingTime();

private:
	void Attack();
	void AttackColliderEnAble();
	void CheckAttackDirection();

	void PlayStandAnimation();


public:
	NormalAttack_State(AI* ai,BJEngine::GameObject* targetobject ,float attackdelay);
	~NormalAttack_State();
};

