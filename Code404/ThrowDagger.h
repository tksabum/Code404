#pragma once

#include "Skill.h"

#include "GameObject.h"
#include "Transform.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Rigidbody2D.h"
#include "Animator.h"

using namespace BJEngine;
using namespace BJEngine::Math;

class ThrowDagger : public Skill
{
public:
	ThrowDagger(Vector2D* userdirection, Transform* usertransform, float* usermp, bool* uniquecost1 = nullptr, bool* uniquecost2 = nullptr);

	// 스킬 사용
	void Use();

private:
	const static float PROJECTILE_SPEED;
	const static float KNIFE_SIZE;
	const static float KNIFE_EFFECT_SIZE;
};

