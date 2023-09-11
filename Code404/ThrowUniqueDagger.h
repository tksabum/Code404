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
using namespace std;

class ThrowUniqueDagger : public Skill
{
public:
	ThrowUniqueDagger(Vector2D* userdirection, Transform* usertransform, float* usermp, bool* uniquecost1 = nullptr, bool* uniquecost2 = nullptr);

	void SetUniqueDagger(GameObject* dagger, wstring effectname);

	// 스킬 사용
	void Use();

private:
	GameObject* uniqueDagger;
	wstring effectName;

	const static float KNIFE_EFFECT_SIZE;
	const static float KNIFE_EFFECT_DISTANCE;
};

