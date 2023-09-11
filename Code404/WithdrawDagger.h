#pragma once

#include "Skill.h"

class WithdrawDagger : public Skill
{
public:
	WithdrawDagger(Vector2D* userdirection, Transform* usertransform, float* usermp, bool* uniquecost1 = nullptr, bool* uniquecost2 = nullptr);

	void SetTargetDagger(GameObject* dagger);

	// ��ų ���
	void Use();

private:
	GameObject* targetDagger;

};

