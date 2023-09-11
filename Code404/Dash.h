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

class Player;

class Dash : public Skill
{
public:
	Dash(Vector2D* userdirection, Transform* usertransform, float* usermp, bool* uniquecost1 = nullptr, bool* uniquecost2 = nullptr);

	void SetPlayer(Player* player);

	// 스킬 사용
	void Use();

private:
	Player* user;

};

