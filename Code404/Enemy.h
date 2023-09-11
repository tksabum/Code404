#pragma once
#include "Component.h"

#include "Code404.h"

namespace BJEngine
{
	class Animator;
}


class AI;
class Boss;

class Enemy :
	public BJEngine::Component
{
private:
	BJEngine::GameObject* hpBarParent;
	BJEngine::GameObject* hpBar;

	float curHP;
	float maxHP = 100;

	bool isAttacked = false;

	AI* ai;

	BJEngine::Animator* anim;

	Boss* bossHeadComponent;
	bool isBossHead = false;

public:
	void SetHPBar(BJEngine::GameObject* hpbar);

	void SetMaxHP(float maxhp);
	void SetCurHP(float curhp);

	void Attacked(PLAYERATTACK_TYPE type, float damage);

	float GetCurHP();
	float GetMaxHP();
	bool IsAttacked();
	void SetIsAttacked(bool isattacked);

	float* GetCurHPAdd();
	float* GetMaxHPAdd();

public:
	virtual void Awake() override;
	virtual void Update() override;

private:
	void Test();

};
